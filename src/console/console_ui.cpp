// =============================================================================
//  console_ui.cpp  -  The TEXT version: menus, drawing the maze, reading keys
// =============================================================================
#include "console_ui.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "game.h"

// Reading one key without Enter works differently on Windows and Linux/macOS.
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <termios.h>
    #include <unistd.h>
#endif

using namespace std;

// ---- Colors ---------------------------------------------------------------------
// "\033[...m" are ANSI codes: they change the color of the text after them.
const string RESET = "\033[0m";
const string RED = "\033[1;91m";
const string GREEN = "\033[1;92m";
const string YELLOW = "\033[1;93m";
const string CYAN = "\033[1;96m";

// Every tile is 2 characters wide, so it looks square.
const string WALL_TEXT   = "\033[44m  " + RESET;        // blue block
const string PLAYER_TEXT = "\033[1;30;106m:)" + RESET;  // black on cyan
const string EXIT_TEXT   = "\033[1;30;103mEX" + RESET;  // black on yellow
const string DOOR_TEXT   = "\033[104m  " + RESET;      // a bit lighter blue than a wall - secret!
const string COIN_TEXT   = YELLOW + "$$" + RESET;
const string TRAIL_TEXT  = "\033[90m ." + RESET;        // dark grey dot

// Special numbers for the arrow keys.
const int ARROW_UP = 1000;
const int ARROW_DOWN = 1001;
const int ARROW_LEFT = 1002;
const int ARROW_RIGHT = 1003;

// Reads ONE key, without waiting for Enter.
int readKey()
{
    cout.flush();
#ifdef _WIN32
    int key = _getch();
    if (key == 0 || key == 224) // arrow keys send 2 codes on Windows
    {
        int arrow = _getch();
        if (arrow == 72) return ARROW_UP;
        if (arrow == 80) return ARROW_DOWN;
        if (arrow == 75) return ARROW_LEFT;
        if (arrow == 77) return ARROW_RIGHT;
        return 0;
    }
    return key;
#else
    // Turn off "wait for Enter" and "show the typed key" for a moment.
    termios oldSettings;
    tcgetattr(STDIN_FILENO, &oldSettings);
    termios newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    int key = getchar();
    if (key == 27 && getchar() == '[') // arrow keys send ESC [ A/B/C/D
    {
        int arrow = getchar();
        if (arrow == 'A') key = ARROW_UP;
        if (arrow == 'B') key = ARROW_DOWN;
        if (arrow == 'C') key = ARROW_RIGHT;
        if (arrow == 'D') key = ARROW_LEFT;
    }
    if (key == EOF)
    {
        key = 'q'; // no more input - quit
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    return key;
#endif
}

// =============================================================================
//  Console setup
// =============================================================================

void prepareConsole()
{
#ifdef _WIN32
    // Windows 10+ understands the ANSI color codes after this is turned on.
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(output, &mode);
    SetConsoleMode(output, mode | 0x0004); // 0x0004 = ENABLE_VIRTUAL_TERMINAL_PROCESSING
#endif
}

void clearScreen()
{
    cout << "\033[2J\033[H"; // clear everything and go to the top-left corner
}

// =============================================================================
//  Menus and screens
// =============================================================================

void showBanner()
{
    cout << GREEN
         << "  +==============================================+\n"
         << "  |    E S C A P E   T H E   M A I N F R A M E   |\n"
         << "  |         a C++ labyrinth adventure            |\n"
         << "  +==============================================+\n"
         << RESET << "\n";
}

void showMainMenu()
{
    clearScreen();
    showBanner();
    cout << "  [1] New game\n"
         << "  [2] How to play\n"
         << "  [3] About\n"
         << "  [0] Exit\n\n";
}

void showHelp()
{
    clearScreen();
    showBanner();
    cout << "  THE STORY\n"
         << "  You are a hacker trapped inside a mainframe. Find the EXIT before\n"
         << "  the system admin notices you!\n\n"
         << "  RULES\n"
         << "  * You start in the top-left corner (position 0,0).\n"
         << "  * The EXIT is always in the bottom-right corner.\n"
         << "  * Walls cannot be crossed...\n"
         << "  * ...except secret BACKDOORS - they always make your way shorter!\n"
         << "  * Collect data chips for bonus points.\n\n"
         << "  LEGEND\n"
         << "  " << PLAYER_TEXT << "  you          " << WALL_TEXT << "  wall\n"
         << "  " << EXIT_TEXT << "  exit         " << DOOR_TEXT << "  secret backdoor (a lighter wall)\n"
         << "  " << COIN_TEXT << "  data chip    " << TRAIL_TEXT << "  your trail\n\n"
         << "  CONTROLS\n"
         << "  Arrow keys or W A S D - move\n"
         << "  H - hint (shows the next step, costs 30 points)\n"
         << "  R - new maze of the same size\n"
         << "  Q - back to the main menu\n\n"
         << "  SCORING\n"
         << "  1000 points, +50 per chip, -5 per extra move, -30 per hint.\n\n"
         << "  Press any key to go back...";
    waitForAnyKey();
}

void showAbout()
{
    clearScreen();
    showBanner();
    cout << "  School project \"Labyrinth\", made in C++ by team CYBERPUNKS.\n\n"
         << "  Every maze is generated randomly with depth-first search,\n"
         << "  and the shortest route is found with breadth-first search.\n\n"
         << "  Press any key to go back...";
    waitForAnyKey();
}

void showVictoryScreen()
{
    int score = calculateScore();

    cout << "\n" << GREEN << "  ************  ACCESS GRANTED - YOU ESCAPED!  ************" << RESET << "\n\n"
         << "  Maze size:        " << mazeSize << " x " << mazeSize << "\n"
         << "  Your moves:       " << moves << "\n"
         << "  Shortest route:   " << optimalMoves << "\n"
         << "  Data chips:       " << coins << "\n"
         << "  Backdoors used:   " << doorsUsed << "\n"
         << "  Hints used:       " << hintsUsed << "\n\n"
         << "  SCORE:  " << YELLOW << score << RESET << "\n"
         << "  RANK:   " << GREEN << getRankTitle(score) << RESET << "\n\n"
         << "  Press any key to return to the main menu...";
    waitForAnyKey();
}

// =============================================================================
//  Input
// =============================================================================

int readNumberInRange(string question, int minValue, int maxValue)
{
    while (true)
    {
        cout << question;
        string line;
        if (!getline(cin, line))
        {
            exit(0); // the input was closed
        }

        if (isWholeNumber(line))
        {
            int value = textToNumber(line);
            if (value >= minValue && value <= maxValue)
            {
                return value;
            }
        }
        cout << RED << "  Invalid input! Please enter a whole number from "
             << minValue << " to " << maxValue << "." << RESET << "\n";
    }
}

int readCommand()
{
    int key = readKey();
    if (key == ARROW_UP || key == 'w' || key == 'W') return COMMAND_UP;
    if (key == ARROW_DOWN || key == 's' || key == 'S') return COMMAND_DOWN;
    if (key == ARROW_LEFT || key == 'a' || key == 'A') return COMMAND_LEFT;
    if (key == ARROW_RIGHT || key == 'd' || key == 'D') return COMMAND_RIGHT;
    if (key == 'h' || key == 'H') return COMMAND_HINT;
    if (key == 'r' || key == 'R') return COMMAND_RESTART;
    if (key == 'q' || key == 'Q') return COMMAND_QUIT;
    return COMMAND_UNKNOWN;
}

void waitForAnyKey()
{
    readKey();
}

// =============================================================================
//  Drawing the game
// =============================================================================

// The 2-character picture of one tile.
string getTileText(int row, int col)
{
    int tile = maze[row][col];
    if (tile == TILE_WALL) return WALL_TEXT;
    if (tile == TILE_DOOR) return DOOR_TEXT;
    if (tile == TILE_COIN) return COIN_TEXT;
    if (tile == TILE_EXIT) return EXIT_TEXT;
    if (visited[row][col]) return TRAIL_TEXT;
    return "  ";
}

string getHintArrow(int direction)
{
    if (direction == DIR_UP) return CYAN + "^^" + RESET;
    if (direction == DIR_DOWN) return CYAN + "vv" + RESET;
    if (direction == DIR_LEFT) return CYAN + "<<" + RESET;
    return CYAN + ">>" + RESET;
}

void drawGame(string message, int hintDirection)
{
    // The whole picture is first built in one string and printed at once,
    // so the screen does not flicker.
    // "\033[K" clears the rest of a line from the previous picture.
    string screen = "\033[H"; // start from the top-left corner
    string size = to_string(mazeSize);
    screen += GREEN + "  ESCAPE THE MAINFRAME" + RESET + "   (" + size + " x " + size + ")\033[K\n";

    // Where to draw the hint arrow
    int hintRow = playerRow + rowChange(hintDirection);
    int hintCol = playerCol + colChange(hintDirection);

    // A border of walls around the maze
    string border = "  ";
    for (int col = 0; col < mazeSize + 2; col++)
    {
        border += WALL_TEXT;
    }
    screen += border + "\033[K\n";

    for (int row = 0; row < mazeSize; row++)
    {
        screen += "  " + WALL_TEXT;
        for (int col = 0; col < mazeSize; col++)
        {
            if (row == playerRow && col == playerCol)
            {
                screen += PLAYER_TEXT;
            }
            else if (hintDirection != DIR_NONE && row == hintRow && col == hintCol)
            {
                screen += getHintArrow(hintDirection);
            }
            else
            {
                screen += getTileText(row, col);
            }
        }
        screen += WALL_TEXT + "\033[K\n";
    }
    screen += border + "\033[K\n";

    // Status line
    screen += "  Moves: " + to_string(moves)
            + "  |  Chips: " + to_string(coins)
            + "  |  Backdoors: " + to_string(doorsUsed)
            + "  |  Hints: " + to_string(hintsUsed)
            + "  |  " + to_string(getStepsToExit()) + " steps to exit\033[K\n";
    screen += "  " + RED + message + RESET + "\033[K\n";
    screen += "  [Arrows/WASD] move  [H] hint  [R] new maze  [Q] menu\033[K\n";
    screen += "\033[J"; // clear everything below

    cout << screen;
    cout.flush();
}

string describeMoveResult(int result)
{
    if (result == MOVE_WALL)    return "BZZT! You can't walk through walls.";
    if (result == MOVE_OUTSIDE) return "There is nothing out there but the void...";
    if (result == MOVE_COIN)    return "+1 data chip! (+50 points)";
    if (result == MOVE_DOOR)    return "You slipped through a secret BACKDOOR - shortcut!";
    if (result == MOVE_EXIT)    return "ACCESS GRANTED!";
    return "";
}

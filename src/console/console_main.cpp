// =============================================================================
//  console_main.cpp  -  Start of the TEXT version: main menu and game loop
// =============================================================================
#include <ctime>
#include <string>

#include "console_ui.h"
#include "game.h"

using namespace std;

// Plays one game until the player wins or presses Q.
void playGame(int size)
{
    startNewGame(size, (unsigned int)time(0));
    string message = "Find the EXIT in the bottom-right corner!";
    int hintDirection = DIR_NONE;

    clearScreen();
    while (true)
    {
        drawGame(message, hintDirection);
        hintDirection = DIR_NONE; // the hint arrow is shown only once

        if (isWon)
        {
            showVictoryScreen();
            break;
        }

        int command = readCommand();
        if (command == COMMAND_QUIT)
        {
            break;
        }
        else if (command == COMMAND_HINT)
        {
            hintDirection = useHint();
            message = "Hint: follow the arrow (-30 points).";
        }
        else if (command == COMMAND_RESTART)
        {
            endGame();
            startNewGame(size, (unsigned int)time(0) + 1);
            message = "A brand new maze has been generated!";
            clearScreen();
        }
        else if (command == COMMAND_UNKNOWN)
        {
            message = "Unknown key. Use the arrows/WASD, H, R or Q.";
        }
        else
        {
            // COMMAND_UP ... COMMAND_RIGHT have the same numbers as DIR_UP ... DIR_RIGHT
            int result = movePlayer(command);
            message = describeMoveResult(result);
        }
    }

    endGame(); // free the memory of the maze
}

int main()
{
    prepareConsole();

    while (true)
    {
        showMainMenu();
        int choice = readNumberInRange("  Choose an option: ", 0, 3);

        if (choice == 0)
        {
            break;
        }
        else if (choice == 1)
        {
            int size = readNumberInRange("  Maze size (10-50): ", MIN_MAZE_SIZE, MAX_MAZE_SIZE);
            playGame(size);
        }
        else if (choice == 2)
        {
            showHelp();
        }
        else if (choice == 3)
        {
            showAbout();
        }
    }

    clearScreen();
    return 0;
}

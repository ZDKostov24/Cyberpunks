// =============================================================================
//  tests.cpp  -  Automatic QA tests for the maze and the game logic
//
//  Every test function checks one rule of the game. The program prints
//  PASS / FAIL for every check and a summary at the end.
// =============================================================================
#include <iostream>
#include <string>

#include "game.h"
#include "maze.h"

using namespace std;

int passedChecks = 0;
int failedChecks = 0;

// Remembers the result of one check and prints it.
void check(bool condition, string description)
{
    if (condition)
    {
        passedChecks++;
        cout << "  [PASS] " << description << "\n";
    }
    else
    {
        failedChecks++;
        cout << "  [FAIL] " << description << "\n";
    }
}

// Builds a small hand-made 10x10 maze, so the tests know where everything is:
//
//   row 0:  S . . $ . . . . . .      S = start, $ = coin
//   row 1:  D # # # # # # # # .      D = door
//   row 2:  . # # # # # # # # .
//   ...                     # .
//   row 9:  # # # # # # # # # E      E = exit
void buildTestMaze()
{
    createMaze(10);
    for (int col = 0; col < 10; col++)
    {
        maze[0][col] = TILE_PATH;   // corridor along the top
    }
    for (int row = 0; row < 10; row++)
    {
        maze[row][9] = TILE_PATH;   // corridor down the right side
    }
    maze[0][3] = TILE_COIN;
    maze[1][0] = TILE_DOOR;
    maze[2][0] = TILE_PATH;
    maze[9][9] = TILE_EXIT;
}

// ---- Checking input ----------------------------------------------------------

void testMazeSizeValidation()
{
    cout << "\nTest: maze size validation\n";
    check(!isValidMazeSize(9), "size 9 is rejected (too small)");
    check(isValidMazeSize(10), "size 10 is accepted (minimum)");
    check(isValidMazeSize(25), "size 25 is accepted");
    check(isValidMazeSize(50), "size 50 is accepted (maximum)");
    check(!isValidMazeSize(51), "size 51 is rejected (too big)");
    check(!isValidMazeSize(-5), "negative size is rejected");
}

void testNumberInput()
{
    cout << "\nTest: checking what the user typed\n";
    check(isWholeNumber("25") && textToNumber("25") == 25, "\"25\" -> 25");
    check(isWholeNumber("  42  ") && textToNumber("  42  ") == 42, "spaces around the number are ignored");
    check(isWholeNumber("-7") && textToNumber("-7") == -7, "\"-7\" -> -7");
    check(!isWholeNumber(""), "empty input is rejected");
    check(!isWholeNumber("abc"), "letters are rejected");
    check(!isWholeNumber("12abc"), "a number followed by letters is rejected");
    check(!isWholeNumber("3.5"), "a decimal number is rejected");
    check(!isWholeNumber("99999999999"), "a huge number is rejected");
}

// ---- The maze --------------------------------------------------------------------

void testMazeMemory()
{
    cout << "\nTest: dynamic memory for the maze\n";
    createMaze(15);
    check(mazeSize == 15, "createMaze stores the size");
    check(maze != nullptr, "createMaze takes memory for the tiles");
    check(countTiles(TILE_WALL) == 15 * 15, "a new maze is full of walls");

    deleteMaze();
    check(maze == nullptr && mazeSize == 0, "deleteMaze frees the memory");
}

void testStartAndExit()
{
    cout << "\nTest: start is (0,0) and exit is in the bottom-right corner (sizes 10-50)\n";
    bool allCorrect = true;
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; size++)
    {
        createMaze(size);
        generateMaze(size * 7 + 1);
        int last = size - 1;
        if (maze[0][0] != TILE_PATH || maze[last][last] != TILE_EXIT || countTiles(TILE_EXIT) != 1)
        {
            allCorrect = false;
        }
        deleteMaze();
    }
    check(allCorrect, "every size has an open start and exactly one exit in the corner");
}

void testEveryMazeCanBeSolved()
{
    cout << "\nTest: every generated maze can be solved\n";
    int solved = 0;
    int total = 0;
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; size++)
    {
        for (int seed = 1; seed <= 5; seed++)
        {
            createMaze(size);
            generateMaze(seed * 1000 + size);
            if (findShortestPathLength(0, 0) > 0)
            {
                solved++;
            }
            total++;
            deleteMaze();
        }
    }
    check(solved == total, to_string(solved) + " of " + to_string(total) + " mazes have a path to the exit");
}

void testDoorsAreShortcuts()
{
    cout << "\nTest: secret doors always make the way shorter\n";
    bool allShorter = true;
    int mazesWithDoors = 0;
    for (int size = 20; size <= MAX_MAZE_SIZE; size += 5)
    {
        createMaze(size);
        generateMaze(size + 99);

        if (countTiles(TILE_DOOR) > 0)
        {
            mazesWithDoors++;
            int withDoors = findShortestPathLength(0, 0);

            // Turn every door back into a wall and measure again.
            for (int row = 0; row < size; row++)
            {
                for (int col = 0; col < size; col++)
                {
                    if (maze[row][col] == TILE_DOOR)
                    {
                        maze[row][col] = TILE_WALL;
                    }
                }
            }
            int withoutDoors = findShortestPathLength(0, 0);

            if (withDoors >= withoutDoors)
            {
                allShorter = false;
            }
        }
        deleteMaze();
    }
    check(mazesWithDoors > 0, "big mazes contain secret doors");
    check(allShorter, "with doors the shortest path is shorter than without them");
}

void testDoorsAreNeverNextToEachOther()
{
    cout << "\nTest: two secret doors are never side by side\n";
    bool noNeighbours = true;
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; size++)
    {
        createMaze(size);
        generateMaze(size * 31);
        for (int row = 0; row < size; row++)
        {
            for (int col = 0; col < size; col++)
            {
                bool isDoor = maze[row][col] == TILE_DOOR;
                bool rightIsDoor = col + 1 < size && maze[row][col + 1] == TILE_DOOR;
                bool belowIsDoor = row + 1 < size && maze[row + 1][col] == TILE_DOOR;
                if (isDoor && (rightIsDoor || belowIsDoor))
                {
                    noNeighbours = false;
                }
            }
        }
        deleteMaze();
    }
    check(noNeighbours, "no door touches another door (sizes 10-50)");
}

void testSameSeedSameMaze()
{
    cout << "\nTest: random generation\n";

    // Remember the maze made with seed 123 in a normal array.
    int first[21][21];
    createMaze(21);
    generateMaze(123);
    for (int row = 0; row < 21; row++)
        for (int col = 0; col < 21; col++)
            first[row][col] = maze[row][col];
    deleteMaze();

    // The same seed again
    bool sameAsFirst = true;
    createMaze(21);
    generateMaze(123);
    for (int row = 0; row < 21; row++)
        for (int col = 0; col < 21; col++)
            if (maze[row][col] != first[row][col]) sameAsFirst = false;
    deleteMaze();

    // Another seed
    bool differentFromFirst = false;
    createMaze(21);
    generateMaze(456);
    for (int row = 0; row < 21; row++)
        for (int col = 0; col < 21; col++)
            if (maze[row][col] != first[row][col]) differentFromFirst = true;
    deleteMaze();

    check(sameAsFirst, "the same seed gives the same maze");
    check(differentFromFirst, "a different seed gives a different maze");
}

// ---- The player ---------------------------------------------------------------------

void testNewGame()
{
    cout << "\nTest: new game\n";
    startNewGame(30, 7);
    check(playerRow == 0 && playerCol == 0, "the player starts at (0,0)");
    check(moves == 0 && coins == 0, "the counters start at 0");
    check(!isWon, "the game is not won at the start");
    check(optimalMoves > 0, "the shortest route is calculated");
    endGame();
}

void testCannotLeaveTheMaze()
{
    cout << "\nTest: the player cannot walk off the map\n";
    buildTestMaze();
    startGame();
    check(movePlayer(DIR_UP) == MOVE_OUTSIDE, "moving up from (0,0) is blocked");
    check(movePlayer(DIR_LEFT) == MOVE_OUTSIDE, "moving left from (0,0) is blocked");
    check(playerRow == 0 && playerCol == 0 && moves == 0, "the player did not move");
    endGame();
}

void testCannotWalkThroughWalls()
{
    cout << "\nTest: walls stop the player\n";
    buildTestMaze();
    startGame();
    movePlayer(DIR_RIGHT); // now at (0,1), a wall is below
    check(movePlayer(DIR_DOWN) == MOVE_WALL, "moving into a wall is blocked");
    check(playerRow == 0 && playerCol == 1, "the player stays in place");
    check(moves == 1, "a blocked move is not counted");
    endGame();
}

void testCollectingCoins()
{
    cout << "\nTest: collecting data chips\n";
    buildTestMaze();
    startGame();
    movePlayer(DIR_RIGHT);
    movePlayer(DIR_RIGHT);
    check(movePlayer(DIR_RIGHT) == MOVE_COIN, "stepping on $ collects it");
    check(coins == 1, "the chip counter goes up");
    check(maze[0][3] == TILE_PATH, "the chip disappears from the map");
    endGame();
}

void testDoorCanBeCrossed()
{
    cout << "\nTest: secret doors can be crossed\n";
    buildTestMaze();
    startGame();
    check(movePlayer(DIR_DOWN) == MOVE_DOOR, "the player walks into the door");
    check(playerRow == 1 && playerCol == 0, "the player stands on the door");
    check(doorsUsed == 1, "the door counter goes up");
    endGame();
}

void testReachingTheExit()
{
    cout << "\nTest: reaching the exit\n";
    buildTestMaze();
    startGame();
    for (int i = 0; i < 9; i++) movePlayer(DIR_RIGHT);
    int lastResult = MOVE_OK;
    for (int i = 0; i < 9; i++) lastResult = movePlayer(DIR_DOWN);

    check(lastResult == MOVE_EXIT, "the last step reaches the exit");
    check(isWon, "the game is won");
    check(movePlayer(DIR_UP) == MOVE_GAME_OVER, "no moves after winning");
    check(moves == optimalMoves, "the perfect route has the optimal number of moves");
    endGame();
}

void testHintsLeadToTheExit()
{
    cout << "\nTest: following the hints solves the maze\n";
    startNewGame(35, 2026);
    int steps = 0;
    while (!isWon && steps < 35 * 35)
    {
        movePlayer(findHintDirection());
        steps++;
    }
    check(isWon, "the hints lead to the exit");
    check(moves == optimalMoves, "the hint path is the shortest one");
    check(findHintDirection() == DIR_NONE, "no hint after winning");
    endGame();
}

void testScore()
{
    cout << "\nTest: score\n";
    buildTestMaze();
    startGame();
    moves = optimalMoves;
    check(calculateScore() == 1000, "perfect run without chips = 1000 points");

    coins = 2;
    check(calculateScore() == 1100, "every chip gives 50 points");

    moves = optimalMoves + 10;
    hintsUsed = 1;
    check(calculateScore() == 1100 - 50 - 30, "extra moves and hints cost points");

    moves = 100000;
    check(calculateScore() == 0, "the score never goes below 0");
    check(getRankTitle(1300) == "LEGENDARY HACKER", "rank title for a top score");
    endGame();
}

int main()
{
    cout << "========== ESCAPE THE MAINFRAME - QA TESTS ==========\n";

    testMazeSizeValidation();
    testNumberInput();
    testMazeMemory();
    testStartAndExit();
    testEveryMazeCanBeSolved();
    testDoorsAreShortcuts();
    testDoorsAreNeverNextToEachOther();
    testSameSeedSameMaze();
    testNewGame();
    testCannotLeaveTheMaze();
    testCannotWalkThroughWalls();
    testCollectingCoins();
    testDoorCanBeCrossed();
    testReachingTheExit();
    testHintsLeadToTheExit();
    testScore();

    cout << "\n=====================================================\n";
    cout << "  Passed: " << passedChecks << "   Failed: " << failedChecks << "\n";

    if (failedChecks == 0)
    {
        return 0;
    }
    return 1;
}

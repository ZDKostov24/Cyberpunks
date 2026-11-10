// =============================================================================
//  game.cpp  -  Game rules: moving the player, items, score, hints
// =============================================================================
#include "game.h"

using namespace std;

int playerRow = 0;
int playerCol = 0;
int moves = 0;
int coins = 0;
int doorsUsed = 0;
int hintsUsed = 0;
int optimalMoves = 0;
bool isWon = false;
bool** visited = nullptr;

// Score rules
const int START_SCORE = 1000;
const int POINTS_PER_COIN = 50;
const int PENALTY_PER_EXTRA_MOVE = 5;
const int PENALTY_PER_HINT = 30;

// =============================================================================
//  Checking user input
// =============================================================================

bool isValidMazeSize(int size)
{
    return size >= MIN_MAZE_SIZE && size <= MAX_MAZE_SIZE;
}

bool isWholeNumber(string text)
{
    // Find where the text really starts and ends (skip the spaces).
    int start = 0;
    int end = (int)text.size();
    while (start < end && text[start] == ' ') start++;
    while (end > start && text[end - 1] == ' ') end--;

    // An optional minus sign
    if (start < end && text[start] == '-')
    {
        start++;
    }

    // Empty text, or too many digits (the number would not fit in an int)
    int digitCount = end - start;
    if (digitCount == 0 || digitCount > 6)
    {
        return false;
    }

    // Every other character must be a digit.
    for (int i = start; i < end; i++)
    {
        if (text[i] < '0' || text[i] > '9')
        {
            return false;
        }
    }
    return true;
}

int textToNumber(string text)
{
    return stoi(text); // stoi = "string to int"
}

// =============================================================================
//  Starting and ending a game
// =============================================================================

void startGame()
{
    playerRow = 0;
    playerCol = 0;
    moves = 0;
    coins = 0;
    doorsUsed = 0;
    hintsUsed = 0;
    isWon = false;

    // A 2D dynamic array that remembers where the player has been.
    visited = new bool*[mazeSize];
    for (int row = 0; row < mazeSize; row++)
    {
        visited[row] = new bool[mazeSize];
        for (int col = 0; col < mazeSize; col++)
        {
            visited[row][col] = false;
        }
    }
    visited[0][0] = true;

    optimalMoves = findShortestPathLength(0, 0);
}

void startNewGame(int size, unsigned int seed)
{
    createMaze(size);
    generateMaze(seed);
    startGame();
}

void endGame()
{
    if (visited != nullptr)
    {
        for (int row = 0; row < mazeSize; row++)
        {
            delete[] visited[row];
        }
        delete[] visited;
        visited = nullptr;
    }

    deleteMaze();
}

// =============================================================================
//  Playing
// =============================================================================

int rowChange(int direction)
{
    if (direction == DIR_UP) return -1;
    if (direction == DIR_DOWN) return 1;
    return 0;
}

int colChange(int direction)
{
    if (direction == DIR_LEFT) return -1;
    if (direction == DIR_RIGHT) return 1;
    return 0;
}

int movePlayer(int direction)
{
    if (isWon)
    {
        return MOVE_GAME_OVER;
    }
    if (direction == DIR_NONE)
    {
        return MOVE_NO_DIRECTION;
    }

    int newRow = playerRow + rowChange(direction);
    int newCol = playerCol + colChange(direction);

    if (!isInsideMaze(newRow, newCol))
    {
        return MOVE_OUTSIDE;
    }
    if (maze[newRow][newCol] == TILE_WALL)
    {
        return MOVE_WALL;
    }

    // The move is allowed.
    playerRow = newRow;
    playerCol = newCol;
    moves++;
    visited[newRow][newCol] = true;

    int tile = maze[newRow][newCol];
    if (tile == TILE_EXIT)
    {
        isWon = true;
        return MOVE_EXIT;
    }
    if (tile == TILE_COIN)
    {
        maze[newRow][newCol] = TILE_PATH; // the chip disappears
        coins++;
        return MOVE_COIN;
    }
    if (tile == TILE_DOOR)
    {
        doorsUsed++;
        return MOVE_DOOR;
    }
    return MOVE_OK;
}

int findHintDirection()
{
    if (isWon)
    {
        return DIR_NONE;
    }

    // Try the 4 neighbours and pick the one that is 1 step closer to the exit.
    int stepsNow = getStepsToExit();
    for (int direction = DIR_UP; direction <= DIR_RIGHT; direction++)
    {
        int row = playerRow + rowChange(direction);
        int col = playerCol + colChange(direction);

        if (isWalkable(row, col) && findShortestPathLength(row, col) == stepsNow - 1)
        {
            return direction;
        }
    }
    return DIR_NONE;
}

int useHint()
{
    int hint = findHintDirection();
    if (hint != DIR_NONE)
    {
        hintsUsed++;
    }
    return hint;
}

int getStepsToExit()
{
    return findShortestPathLength(playerRow, playerCol);
}

int calculateScore()
{
    int extraMoves = moves - optimalMoves;
    if (extraMoves < 0)
    {
        extraMoves = 0;
    }

    int score = START_SCORE
              + coins * POINTS_PER_COIN
              - extraMoves * PENALTY_PER_EXTRA_MOVE
              - hintsUsed * PENALTY_PER_HINT;

    if (score < 0)
    {
        score = 0;
    }
    return score;
}

string getRankTitle(int score)
{
    if (score >= 1200) return "LEGENDARY HACKER";
    if (score >= 1000) return "Elite Netrunner";
    if (score >= 800)  return "Code Ninja";
    if (score >= 500)  return "Script Kiddie";
    if (score >= 200)  return "Lost Packet";
    return "Blue Screen of Death";
}

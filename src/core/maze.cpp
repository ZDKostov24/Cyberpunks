// =============================================================================
//  maze.cpp  -  The maze: creating it, generating it, finding the shortest path
// =============================================================================
#include "maze.h"

#include <cstdlib>

int mazeSize = 0;
int** maze = nullptr;

// The 4 directions as row/column steps: up, down, left, right.
const int ROW_STEP[4] = { -1, 1, 0, 0 };
const int COL_STEP[4] = { 0, 0, -1, 1 };

// =============================================================================
//  Creating and deleting
// =============================================================================

void createMaze(int size)
{
    mazeSize = size;

    // A 2D dynamic array: first an array of rows, then every row.
    maze = new int*[size];
    for (int row = 0; row < size; row++)
    {
        maze[row] = new int[size];
        for (int col = 0; col < size; col++)
        {
            maze[row][col] = TILE_WALL;
        }
    }
}

void deleteMaze()
{
    if (maze == nullptr)
    {
        return; // there is nothing to delete
    }

    for (int row = 0; row < mazeSize; row++)
    {
        delete[] maze[row];
    }
    delete[] maze;

    maze = nullptr;
    mazeSize = 0;
}

// =============================================================================
//  Simple questions about the maze
// =============================================================================

bool isInsideMaze(int row, int col)
{
    return row >= 0 && row < mazeSize && col >= 0 && col < mazeSize;
}

bool isWalkable(int row, int col)
{
    return isInsideMaze(row, col) && maze[row][col] != TILE_WALL;
}

int countTiles(int tile)
{
    int count = 0;
    for (int row = 0; row < mazeSize; row++)
    {
        for (int col = 0; col < mazeSize; col++)
        {
            if (maze[row][col] == tile)
            {
                count++;
            }
        }
    }
    return count;
}

// =============================================================================
//  Shortest path - Breadth-First Search (BFS)
//
//  We start at (row, col) and visit the maze in "rings": first all tiles
//  1 step away, then 2 steps away, and so on. A queue remembers which tiles
//  to visit next. When we reach the exit, we know the shortest way to it.
// =============================================================================

int findShortestPathLength(int row, int col)
{
    if (!isWalkable(row, col))
    {
        return -1;
    }

    // distance[r][c] = steps from the start to (r, c); -1 = not visited yet
    int** distance = new int*[mazeSize];
    for (int r = 0; r < mazeSize; r++)
    {
        distance[r] = new int[mazeSize];
        for (int c = 0; c < mazeSize; c++)
        {
            distance[r][c] = -1;
        }
    }

    // The queue: two arrays (row and column) and two positions in them.
    int* queueRow = new int[mazeSize * mazeSize];
    int* queueCol = new int[mazeSize * mazeSize];
    int first = 0; // the next tile to take out
    int last = 0;  // the next free place

    distance[row][col] = 0;
    queueRow[last] = row;
    queueCol[last] = col;
    last++;

    while (first < last)
    {
        int currentRow = queueRow[first];
        int currentCol = queueCol[first];
        first++;

        for (int direction = 0; direction < 4; direction++)
        {
            int nextRow = currentRow + ROW_STEP[direction];
            int nextCol = currentCol + COL_STEP[direction];

            if (isWalkable(nextRow, nextCol) && distance[nextRow][nextCol] == -1)
            {
                distance[nextRow][nextCol] = distance[currentRow][currentCol] + 1;
                queueRow[last] = nextRow;
                queueCol[last] = nextCol;
                last++;
            }
        }
    }

    int exitDistance = distance[mazeSize - 1][mazeSize - 1];

    // Free all the memory we took.
    for (int r = 0; r < mazeSize; r++)
    {
        delete[] distance[r];
    }
    delete[] distance;
    delete[] queueRow;
    delete[] queueCol;

    return exitDistance;
}

// =============================================================================
//  Generating a random maze
// =============================================================================

// Digs corridors with a RECURSIVE depth-first search.
//
// "Rooms" are the tiles with even coordinates: (0,0), (0,2), (2,0) ...
// From the current room we try the 4 directions in random order. If the room
// 2 tiles away is still a wall, we break the wall between us and dig from there.
// When no direction works, the function returns and we continue from the
// previous room. In the end every room is connected - the maze can be solved.
void digPassages(int row, int col)
{
    maze[row][col] = TILE_PATH;

    // Shuffle the 4 directions.
    int directions[4] = { 0, 1, 2, 3 };
    for (int i = 3; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int temp = directions[i];
        directions[i] = directions[j];
        directions[j] = temp;
    }

    for (int i = 0; i < 4; i++)
    {
        int direction = directions[i];
        int nextRow = row + 2 * ROW_STEP[direction];
        int nextCol = col + 2 * COL_STEP[direction];

        if (isInsideMaze(nextRow, nextCol) && maze[nextRow][nextCol] == TILE_WALL)
        {
            // Break the wall between the two rooms and continue from the new room.
            maze[row + ROW_STEP[direction]][col + COL_STEP[direction]] = TILE_PATH;
            digPassages(nextRow, nextCol);
        }
    }
}

// Puts the exit in the bottom-right corner.
void placeExit()
{
    int last = mazeSize - 1;
    maze[last][last] = TILE_EXIT;

    // With an even size the corner is not a "room", so we also open the tile
    // above it. That tile touches the room at (last - 1, last - 1).
    if (mazeSize % 2 == 0)
    {
        maze[last - 1][last] = TILE_PATH;
    }
}

// true if the wall at (row, col) could become a door: there must be corridor
// on both sides of it, and no other door right next to it.
bool canBeDoor(int row, int col)
{
    if (maze[row][col] != TILE_WALL)
    {
        return false;
    }

    for (int direction = 0; direction < 4; direction++)
    {
        int nextRow = row + ROW_STEP[direction];
        int nextCol = col + COL_STEP[direction];
        if (isInsideMaze(nextRow, nextCol) && maze[nextRow][nextCol] == TILE_DOOR)
        {
            return false;
        }
    }

    bool corridorUpAndDown = isWalkable(row - 1, col) && isWalkable(row + 1, col);
    bool corridorLeftAndRight = isWalkable(row, col - 1) && isWalkable(row, col + 1);
    return corridorUpAndDown || corridorLeftAndRight;
}

// Turns a few walls into secret doors. A wall becomes a door ONLY if it makes
// the way from the start to the exit shorter (task requirement).
void placeDoors()
{
    int wantedDoors = mazeSize / 5;
    int placedDoors = 0;
    int maxAttempts = mazeSize * mazeSize;

    for (int attempt = 0; attempt < maxAttempts && placedDoors < wantedDoors; attempt++)
    {
        int row = 1 + rand() % (mazeSize - 2);
        int col = 1 + rand() % (mazeSize - 2);
        if (!canBeDoor(row, col))
        {
            continue;
        }

        // Try it: open the door and measure the way again.
        int lengthBefore = findShortestPathLength(0, 0);
        maze[row][col] = TILE_DOOR;
        int lengthAfter = findShortestPathLength(0, 0);

        if (lengthAfter < lengthBefore)
        {
            placedDoors++;              // it is a shortcut - keep it
        }
        else
        {
            maze[row][col] = TILE_WALL; // not a shortcut - undo
        }
    }
}

// Puts data chips (coins) on random corridor tiles.
void placeCoins()
{
    int wantedCoins = mazeSize * mazeSize / 50;
    int placedCoins = 0;
    int maxAttempts = mazeSize * mazeSize * 10; // never loop forever

    for (int attempt = 0; attempt < maxAttempts && placedCoins < wantedCoins; attempt++)
    {
        int row = rand() % mazeSize;
        int col = rand() % mazeSize;
        bool isStart = (row == 0 && col == 0);

        if (!isStart && maze[row][col] == TILE_PATH)
        {
            maze[row][col] = TILE_COIN;
            placedCoins++;
        }
    }
}

void generateMaze(unsigned int seed)
{
    srand(seed);

    // Start with walls everywhere.
    for (int row = 0; row < mazeSize; row++)
    {
        for (int col = 0; col < mazeSize; col++)
        {
            maze[row][col] = TILE_WALL;
        }
    }

    digPassages(0, 0); // the player starts at (0, 0)
    placeExit();
    placeDoors();
    placeCoins();
}

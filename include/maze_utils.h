#ifndef MAZE_UTILS_H
#define MAZE_UTILS_H

#include <stdbool.h>
#include <stdlib.h>

// Bitmask Directions
#define UP 0b0001    // 0001
#define DOWN 0b0010  // 0010
#define RIGHT 0b0100 // 0100
#define LEFT 0b1000  // 1000

typedef struct {
  int width;
  int height;
  int *cells;
  bool *visited;
} Maze;

extern bool mazeGeneratorStarted;

bool mazeFinished(Maze *grid, size_t history_size);
void render_maze(Maze *maze, float tileWidth, float tileHeight, int columns,
                 int rows, int tracker, int *history, size_t history_size);

#endif

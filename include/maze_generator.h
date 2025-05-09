#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include "maze_utils.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Maze *create_maze(int width, int height);
int getValidDirections(int tracker, Maze *grid);
void mazeGeneratorStep(Maze *grid);
void draw_maze(Maze *grid, float tileWidth, float tileHeight, int columns,
               int rows);
bool mazeGenerated(Maze *grid);

#endif

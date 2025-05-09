#include "maze_generator.h"
#include "maze_utils.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int *history;
size_t history_size;
int tracker;

Maze *create_maze(int width, int height) {
  Maze *grid = (Maze *)malloc(sizeof(Maze));

  if (grid == NULL) {
    fprintf(stderr, "Memory for the Maze could not be initialized.\n");
    return NULL;
  }

  grid->cells = (int *)malloc(width * height * sizeof(int));

  if (grid->cells == NULL) {
    fprintf(stderr, "Memory Allocation for Maze Cells Failed.\n");
    return NULL;
  }

  grid->width = width;
  grid->height = height;
  grid->visited = (bool *)malloc(width * height * sizeof(bool));

  if (grid->visited == NULL) {
    fprintf(stderr, "Memory Allocation for visited Cells Failed.\n");
    return NULL;
  }

  for (int i = 0; i < width * height; i++) {
    grid->visited[i] = false;
    grid->cells[i] = UP | DOWN | RIGHT | LEFT;
  }

  return grid;
}

int getValidDirections(int tracker, Maze *grid) {
  int valid = 0;
  int x = tracker % grid->width;
  int y = tracker / grid->width;

  if (y > 0 && !grid->visited[tracker - grid->width]) {
    valid |= UP;
  }
  if (x + 1 < grid->width && !grid->visited[tracker + 1]) {
    valid |= RIGHT;
  }
  if (y + 1 < grid->height && !grid->visited[tracker + grid->width]) {
    valid |= DOWN;
  }
  if (x > 0 && !grid->visited[tracker - 1]) {
    valid |= LEFT;
  }

  return valid;
}

void mazeGeneratorStep(Maze *grid) {

  if (!mazeGeneratorStarted) {
    printf("Maze Generation Started\n");
    history = (int *)malloc(grid->width * grid->height * sizeof(int));

    if (history == NULL) {
      fprintf(stderr, "Memory Allocation of Backtracker History Failed.\n");
      return;
    }

    history_size = 0;
    srand(time(NULL));
    tracker = 0; // At which index the Tracker is
    history[history_size] = tracker;
    history_size++;
    grid->visited[tracker] = true;
    mazeGeneratorStarted = true;
  } else {
    int valid_directions = getValidDirections(tracker, grid);

    if (valid_directions != 0b0000) {
      int dirs[4] = {UP, DOWN, RIGHT, LEFT};
      int opposites[4] = {DOWN, UP, LEFT, RIGHT};
      int offset[4] = {-grid->width, grid->width, 1, -1};

      int pick;
      do {
        pick = rand() % 4;
      } while (!(valid_directions & dirs[pick]));

      history[history_size] = tracker;
      history_size += 1;

      grid->cells[tracker] &= ~dirs[pick];
      tracker += offset[pick];
      grid->cells[tracker] &= ~opposites[pick];

      grid->visited[tracker] = true;

    } else if (valid_directions == 0b0000) {

      history_size--;
      if (history_size > 0) {

        tracker = history[history_size];
      }
    }
  }
}

void draw_maze(Maze *grid, float tileWidth, float tileHeight, int columns,
               int rows) {
  render_maze(grid, tileWidth, tileHeight, columns, rows, tracker, history,
              history_size);
}

bool mazeGenerated(Maze *grid) { return mazeFinished(grid, history_size); }

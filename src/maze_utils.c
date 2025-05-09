#include "maze_utils.h"
#include <raylib.h>
#include <stdbool.h>

bool mazeGeneratorStarted = false;

bool mazeFinished(Maze *grid, size_t history_size) {
  if (mazeGeneratorStarted && history_size == 0) {
    return true;
  } else {
    return false;
  }
}

void render_maze(Maze *maze, float tileWidth, float tileHeight, int columns,
                 int rows, int tracker, int *history, size_t history_size) {
  BeginDrawing();
  ClearBackground(RAYWHITE);
  for (int i = 0; i < maze->width; i++) {
    for (int j = 0; j < maze->height; j++) {

      Color color = LIME;

      bool isVisited = maze->visited[i + j * maze->width];
      int cell = maze->cells[i + j * maze->width];
      float x = tileWidth * i;
      float y = tileHeight * j;

      if (isVisited) {
        color = RED;
      } else {
        color = WHITE;
      }
      for (int d = 0; d < history_size; ++d) {
        if (i + j * columns == history[d]) {
          color = LIME;
        }
      }

      if (!mazeFinished(maze, history_size) && i + j * columns == tracker) {
        color = BLUE;
      }

      DrawRectangle(x, y, tileWidth, tileHeight, color);

      color = BLACK;

      Vector2 upLeft = {x, y};
      Vector2 upRight = {x + tileWidth, y};
      Vector2 bottomLeft = {x, y + tileHeight};
      Vector2 bottomRight = {x + tileWidth, y + tileHeight};

      if (cell & UP) {
        DrawLineEx(upLeft, upRight, 2, color);
      }
      if (cell & DOWN) {
        DrawLineEx(bottomLeft, bottomRight, 2, color);
      }
      if (cell & RIGHT) {
        DrawLineEx(upRight, bottomRight, 2, color);
      }
      if (cell & LEFT) {
        DrawLineEx(upLeft, bottomLeft, 2, color);
      }
    }
  }
  EndDrawing();
}

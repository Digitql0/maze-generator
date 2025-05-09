#include "maze_generator.h"
#include "maze_utils.h"
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>

const int width = 800;
const int height = 800;
const int rows = 20;
const int columns = 20;
const float tileHeight = (float)height / rows;
const float tileWidth = (float)width / columns;

int main() {

  InitWindow(width, height, "MAZES");
  SetTargetFPS(60);

  printf("Program Started.\n");
  Maze *maze = create_maze(columns, rows);

  while (!WindowShouldClose()) {

    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (!mazeGenerated(maze)) {
      mazeGeneratorStep(maze);
    }
    draw_maze(maze, tileWidth, tileWidth, columns, rows);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

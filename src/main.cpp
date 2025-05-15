#include "maze.hpp"
#include <iostream>
#include <raylib.h>

const int width = 1000;
const int height = 1000;
const int rows = 250;
const int columns = 250;
const float tileHeight = (float)height / rows;
const float tileWidth = (float)width / columns;
const char* title = "Maze Generator and Solver";
Vector2 goal = {rows-5, columns-5};
Vector2 start = {1, 1};

int main() {
	srand(time(NULL));
	InitWindow(width, height, title);
	SetTargetFPS(60);

	std::cout << "Program Started" << std::endl;
	Maze maze(rows, columns, goal, start);
	std::cout << "Maze was made" << std::endl;

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(RAYWHITE);
		while (!maze.isGenerated()) {
			maze.generateStep();
		}
		while (maze.isGenerated() && !maze.isSolved()) {
			std::cout << "Maze Solving Step ..." << std::endl;
			maze.bfs();
			std::cout << "Maze Solving Step done" << std::endl;
		}
		
		std::cout << "done" << std::endl;
		maze.render(tileWidth, tileHeight);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

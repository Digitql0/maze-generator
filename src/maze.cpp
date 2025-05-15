#include "maze.hpp"
#include <cstdlib>
#include <iostream>
#include <raylib.h>
#include <vector>
#include <array>

Maze::Maze(int rows, int columns, Vector2 goal, Vector2 start) : generated(false), rows(rows), columns(columns), goal(goal), start(start), solved(false),solutionsMade(false){

	//Make a new Grid of Cells for the Maze
	std::vector<std::vector<Cell>> buffer_grid;
	for ( int i = 0; i < rows; ++i) {
		std::vector<Cell> buffer_cell_vector;
		for (int j = 0; j < columns; ++j) {
			Cell buffer_cell;
			buffer_cell.up = true;
			buffer_cell.down = true;
			buffer_cell.left = true;
			buffer_cell.right = true;
			buffer_cell.generated = false;
			buffer_cell.visited = false;
			buffer_cell_vector.push_back(buffer_cell);
		}
		buffer_grid.push_back(buffer_cell_vector);
	}
	grid = buffer_grid;

	//Make the Tracker
	generator = Tracker();
	generator.history = {};
	generator.curr_pos.x = 0; // Upper left Corner of the Maze
	generator.curr_pos.y = 0;

	//Make the generator Start generated already
	grid[0][0].generated = true;
}

bool Maze::isGenerated() {
	return generated;
}

void Maze::generateStep() {
	Vector2 up = {0, -1};
	Vector2 down = {0, 1};
	Vector2 left = {-1, 0};
	Vector2 right = {1, 0};

	//Array of Directions the Tracker can go in
	//picked at Random and if not possible discarded.
	std::vector<Vector2> offsets = {up, down, left, right};

	std::cout << "loop" << std::endl;

	do {
		int pick = rand() % offsets.size();

		Vector2 picked_offset = offsets[pick];

		int dx = generator.curr_pos.x + picked_offset.x;
		int dy = generator.curr_pos.y + picked_offset.y;
		Cell& curr_cell = grid[generator.curr_pos.y][generator.curr_pos.x];

		bool xOOB = -1 < dx && dx < columns;
		bool yOOB = -1 < dy && dy < rows;

		std::cout << "hit" << std::endl;

		if (xOOB && yOOB && !grid[dy][dx].generated) {
			//Remve the Wall the way the generator is moving
			remove_wall(curr_cell, grid[dy][dx], picked_offset);

			//Add current position to the History of the generator
			generator.history.push_back(generator.curr_pos);

			//Offset the generator
			generator.curr_pos.x = dx;
			generator.curr_pos.y = dy;

			grid[dy][dx].generated = true;
			break;
		} else {
			offsets.erase(offsets.begin() + pick);
		}

	} while (offsets.size() > 0);

	if (offsets.size() <= 0) {


		//No Valid Directions were Found

		if (generator.history.size() <= 0) {
			generated = true;
		}

		else {
		Vector2 popped_position = generator.history.back();
		generator.history.pop_back();
		generator.curr_pos = popped_position;
		}
	}
}

void Maze::remove_wall(Cell &curr, Cell &neighbour, Vector2 dir) {
	Vector2 up = {0, -1};
	Vector2 down = {0, 1};
	Vector2 left = {-1, 0};
	Vector2 right = {1, 0};

	if (dir.y == up.y) {
		curr.up = false;
		neighbour.down = false;
	}
	else if (dir.y == down.y) {
		curr.down = false;
		neighbour.up = false;
	}
	else if (dir.x == left.x) {
		curr.left = false;
		neighbour.right = false;
	}
	else if (dir.x == right.x) {
		curr.right = false;
		neighbour.left = false;
	}
}

void Maze::render(float tileWidth, float tileHeight) {
	for (int a = 0; a < rows; ++a) {
		for (int b = 0; b < columns; ++b) {
			Color color = LIME;

			Cell curr_cell = grid[a][b];
			bool curr_generated = curr_cell.generated;

			if (curr_generated) {
				color = RED; //Make generated Cells Red
			} else {
				color = WHITE; //and not generated Cells White
			}
			
			//Make the Tracker Blue and its history lime, while not generated fully
			if (!isGenerated() && generator.curr_pos.x == b && generator.curr_pos.y == a) {
				color = BLUE;
			} else {

				for (int x = 0; x < generator.history.size(); ++x) {
					Vector2 cell = generator.history[x];

					if (cell.x == b && cell.y == a) {
						color = LIME;
						break;
					}
				}
			}

			if (isGenerated()) {
				if (goal.x == b && goal.y == a) {
					color = YELLOW;
				}
			}

			//Make the Agents blue and its Histories Lime, while not solved but generated
			if (!isSolved() && isGenerated()) {
				bool painted = false;
				for (auto &tracker : trackers) {
					// head of tracker
					if (tracker.curr_pos.x == b && tracker.curr_pos.y == a) {
						color = BLUE;
						painted = true;
						break;
					} else {
						// in its history
						for (auto &h : tracker.history) {
							if (h.x == b && h.y == a) {
								color = LIME;
								painted = true;
								break;
							}
						}
					}
					if (painted) break;
				}
			}

			if (isSolved() && isGenerated()) {
				bool painted = false;
				for (auto &tracker : solutions) {
					// head of tracker
					if (tracker.curr_pos.x == b && tracker.curr_pos.y == a) {
						color = BLUE;
						painted = true;
						break;
					} else {
						// in its history
						for (auto &h : tracker.history) {
							if (h.x == b && h.y == a) {
								color = LIME;
								painted = true;
								break;
							}
						}
					}
					if (painted) break;
				}
			}

			float x = b * tileWidth;
			float y = a * tileHeight;

			DrawRectangle(x, y, tileWidth, tileHeight, color);

			color = BLACK;

			Vector2 upLeft = {x, y};
			Vector2 upRight = {x + tileWidth, y};
			Vector2 bottomLeft = {x, y + tileHeight};
			Vector2 bottomRight = {x + tileWidth, y + tileHeight};

			if (curr_cell.up) {
				DrawLineEx(upLeft, upRight, 2, color);
			}
			if (curr_cell.down) {
				DrawLineEx(bottomLeft, bottomRight, 2, color);
			}
			if (curr_cell.right) {
				DrawLineEx(upRight, bottomRight, 2, color);
			}
			if (curr_cell.left) {
				DrawLineEx(upLeft, bottomLeft, 2, color);
			}
		}
	}
}

bool Maze::check_wall(const Cell &curr, Vector2 dir) {
	Vector2 up = {0, -1};
	Vector2 down = {0, 1};
	Vector2 left = {-1, 0};
	Vector2 right = {1, 0};

	if (dir.y == up.y && curr.up) {
		return true;
	}
	else if (dir.y == down.y && curr.down) {
		return true;
	}
	else if (dir.x == left.x && curr.left) {
		return true;
	}
	else if (dir.x == right.x && curr.right) {
		return true;
	}

	return false;
}

void Maze::bfs() {

	std::cout << "solved: " << solved << std::endl;

	// Initialize first Tracker if Empty
	if (trackers.empty()) {
		std::cout << "Initializing Trackers" << std::endl;
		Tracker origin;
		origin.curr_pos = start;
		grid[start.y][start.x].visited = true;
		trackers.push_back(origin);
		solutions.clear();
	}

	if (solved && solutions.empty()) {
		std::cout << "Goal reached—storing solution(s)\n" << std::endl;

		for (auto &t : trackers) {
			if (t.curr_pos.x == goal.x && t.curr_pos.y == goal.y) {
				// copy rather than move, so trackers remains intact if you ever
				// want to inspect them
				solutions.push_back(t);
			}
		}

		solutionsMade = true;
	}

	if (solved) {
		std::cout << "FUCK THIS SHIT" << std::endl;
		return;
	}

	std::array<Vector2, 4> dirs = {{
		{ 0, -1 },  // up
		{ 0,  1 },  // down
		{-1,  0 },  // left
		{ 1,  0 },  // right
	}};

	std::vector<Tracker> next_trackers;

	for (auto &curr : trackers) {
		std::cout << "Trackers Loop" << std::endl;
		int cx = curr.curr_pos.x;
		int cy = curr.curr_pos.y;

		for (auto &dir : dirs) {
			std::cout << "Direction Loop" << std::endl;
			int dx = cx + dir.x;
			int dy = cy + dir.y;

			// Bounds Check

			// skip if any test fails
			if (dx < 0 || dx >= columns || dy < 0 || dy >= rows) {
				continue;
			}
			std::cout << "Bounds Checked" << std::endl;

			// visited check
			if (grid[dy][dx].visited) {
				continue;
			}
			std::cout << "visit Checked" << std::endl;


			const Cell &cell = grid[cy][cx];

			// Wall Check
			if (check_wall(cell, dir)) {
				continue;
			}

			std::cout << "Wall Checked" << std::endl;

			//Make the new Tracker
			Tracker new_tracker = curr;
			new_tracker.curr_pos = {(float)dx, (float)dy};
			new_tracker.history.push_back(curr.curr_pos);
			grid[dy][dx].visited = true;

			if (dx == goal.x && dy == goal.y) {
				solved = true;
				solutions.clear();
			}

			next_trackers.push_back(std::move(new_tracker));
			std::cout << "new_tracker 'moved'" << std::endl;
		}
	}


	trackers = std::move(next_trackers);
	std::cout << "trackers 'moved'" << std::endl;

	if (trackers.empty()) {
		solved = true;
		std::cout << "Trackers are Empty" << std::endl;
	}
}

void Maze::dijkstra() {return;}
void Maze::a_star() {return;}

bool Maze::goal_reached(std::vector<Tracker> list) {
	for (int i = 0; i < list.size(); ++i) {
		if (list[i].curr_pos.x == goal.x && list[i].curr_pos.y == goal.y) {
			return true;
		}
	}

	return false;
}

bool Maze::isSolved() {
	return solutionsMade;
}

#ifndef MAZE_HPP
#define MAZE_HPP

#include <raylib.h>
#include <vector>

typedef struct {
	bool up;
	bool down;
	bool left;
	bool right;
	bool generated;
	bool visited;
} Cell;

typedef struct {
	std::vector<Vector2> history;
	Vector2 curr_pos;
} Tracker;

class Maze {
public:
	//Initializing
	Maze(int rows, int columns, Vector2 goal, Vector2 start);
	
	//Generating
	bool isGenerated();
	bool isSolved();
	void generateStep();
	
	//Drawing
	void render(float tileWidth, float tileHeight);

	//Solving
	void bfs();
	void dijkstra();
	void a_star();
private:
	void remove_wall(Cell &curr, Cell &neighbour, Vector2 dir);
	bool check_wall(const Cell &curr, Vector2 dir);
	bool goal_reached(std::vector<Tracker> list);
	std::vector<std::vector<Cell>> grid;
	Tracker generator;
	bool generated;
	bool solved;
	bool solutionsMade;
	int rows;
	int columns;
	Vector2 goal;
	Vector2 start;
	unsigned int dfs_fps = 0;
	std::vector<Tracker> trackers;
	std::vector<Tracker> solutions;
};

#endif

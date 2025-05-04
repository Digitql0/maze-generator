#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int width = 800;
const int height = 800;
const int rows = 20;
const int columns = 20;
const float tileHeight = (float)height / rows;
const float tileWidth = (float)width / columns;
const int initialAgencyCapacity = 10;
const int initialHistoryCapacity = 10;

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

typedef struct {
  int pos;
  int *history;
  size_t history_size;
  size_t history_capacity;
} Agent;

typedef struct {
  Agent *agents;
  size_t agency_size;
  size_t agency_capacity;
} Agency;

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

int *history;
size_t history_size;
int tracker;
bool mazeGeneratorStarted = false;

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

bool mazeFinished(Maze *grid) {
  if (mazeGeneratorStarted && history_size == 0) {
    return true;
  } else {
    return false;
  }
}

int getValidSteps(Agent agent, Maze *grid) {
  int valid = 0;
  int pos = agent.pos;
  int x = pos % grid->width;
  int y = pos / grid->width;
  int pre_pos = agent.history[agent.history_size - 1];

  int up = pos - grid->width;
  int down = pos + grid->width;
  int right = pos + 1;
  int left = pos - 1;

  if (pre_pos != up && y > 0 && !(grid->cells[up] & DOWN)) {
    valid |= UP;
  }
  if (pre_pos != right && x + 1 < grid->width && !(grid->cells[right] & LEFT)) {
    valid |= RIGHT;
  }
  if (pre_pos != down && y + 1 < grid->height && !(grid->cells[down] & UP)) {
    valid |= DOWN;
  }
  if (pre_pos != left && x > 0 && !(grid->cells[left] & RIGHT)) {
    valid |= LEFT;
  }

  return valid;
}

bool mazeSolverStarted = false;

Agency *agency;

void mazeSolvingStep(Maze *grid) {
  if (!mazeSolverStarted) {

    agency = (Agency *)malloc(sizeof(Agency));

    if (agency == NULL) {
      fprintf(stderr, "Agency wasnt able to be Allocated.\n");
      return;
    }

    agency->agents = (Agent *)malloc(initialAgencyCapacity * sizeof(Agent));

    if (agency->agents == NULL) {
      fprintf(stderr, "Agents werent able to be Allocated.\n");
      return;
    }

    agency->agency_size = 1;
    agency->agency_capacity = 10;

    agency->agents[0].pos = 0;
    agency->agents[0].history =
        (int *)malloc(initialHistoryCapacity * sizeof(int));

    if (agency->agents[0].history == NULL) {
      fprintf(stderr, "History of First Agents could not be Allocated.\n");
      return;
    }

    agency->agents[0].history[0] = 0;
    agency->agents[0].history_size = 1;
    agency->agents[0].history_capacity = initialHistoryCapacity;

  } else {
    for (int i = 0; i < agency->agency_size; ++i) {
      int valid_steps = getValidSteps(agency->agents[0], grid);

      if (valid_steps != 0b0000) {

        int directions[4] = {UP, DOWN, RIGHT, LEFT};
        int offsets[4] = {-grid->width, grid->width, 1, -1};

        int count_valid_steps = 0;

        for (int j = 0; j < 4; ++j) {
          if (valid_steps & directions[j]) {
            count_valid_steps++;
          }
        }

        Agency *bureau = (Agency *)malloc(sizeof(Agency));

        if (bureau == NULL) {
          fprintf(stderr, "Bureau could not be Allocated.\n");
          return;
        }

        bureau->agents = (Agent *)malloc(count_valid_steps * sizeof(Agent));

        if (bureau->agents == NULL) {
          fprintf(stderr, "Agents of Bureau could not be Allocated.\n");
          return;
        }

        bureau->agency_capacity = count_valid_steps;
        bureau->agency_size = 0;

        for (int j = 0; j < count_valid_steps; ++j) {
          bureau->agents[bureau->agency_size] = agency->agents[i];
          if (bureau->agents[bureau->agency_size].history_size >=
              bureau->agents[bureau->agency_size].history_capacity) {
            bureau->agents[bureau->agency_size].history = (int *)realloc(
                bureau->agents[bureau->agency_size].history,
                bureau->agents[bureau->agency_size].history_size +
                    1 * sizeof(int));
            if (bureau->agents[bureau->agency_size].history == NULL) {
              fprintf(stderr, "New Agent History could not be Allocated.\n");
              return;
            }
            bureau->agents[bureau->agency_size].history_capacity =
                bureau->agents[bureau->agency_size].history_size + 1;
          }
          bureau->agents[bureau->agency_size]
              .history[bureau->agents[bureau->agency_size].history_size] =
              agency->agents[i].pos;
          bureau->agents[bureau->agency_size].history_size++;
          bureau->agency_size++;
        }

        int agent_index = 0;
        for (int j = 0; j < 4; ++j) {
          if (valid_steps & directions[j]) {
            bureau->agents[agent_index].pos += offsets[j];
            agent_index++;
          }
        }

      } else {
      }
    }
  }
}

void render_maze(Maze *maze) {
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

      if (!mazeFinished(maze) && i + j * columns == tracker) {
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

int main() {

  InitWindow(width, height, "MAZES");
  SetTargetFPS(60);

  printf("Program Started.\n");
  Maze *maze = create_maze(columns, rows);

  while (!WindowShouldClose()) {

    BeginDrawing();
    ClearBackground(RAYWHITE);

    if (!mazeFinished(maze)) {
      printf("Maze is getting Generated\n");
      mazeGeneratorStep(maze);
    } else {
      printf("Maze is getting solved\n");
      // mazeSolvingStep(maze);
    }

    printf("size: %zu", history_size);

    render_maze(maze);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

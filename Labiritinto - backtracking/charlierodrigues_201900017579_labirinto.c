#include <stdio.h>
#include <stdlib.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

typedef struct {
    int x;
    int y;
} Coordinate;

typedef struct {
    Coordinate start;
    Coordinate end;
    int width;
    int height;
    int maze[MAX_HEIGHT][MAX_WIDTH];
} Maze;

void readMaze(Maze *maze) {
    int i, j;
    scanf("%d %d", &maze->width, &maze->height);
    for (i = 0; i < maze->height; i++) {
        for (j = 0; j < maze->width; j++) {
            scanf("%d", &maze->maze[i][j]);
            if (maze->maze[i][j] == 'X') {
                maze->start.x = i;
                maze->start.y = j;
            }
        }
    }
}

void printPath(Coordinate *path, int length) {
    int i;
    for (i = 0; i < length; i++) {
        printf("%c@%d,%d", i == 0 ? "INICIO" : i == length - 1 ? "SAIDA" : "", path[i].x, path[i].y);
        if (i < length - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int isValidMove(Maze *maze, int x, int y) {
    return x >= 0 && x < maze->height && y >= 0 && y < maze->width && maze->maze[x][y] != 1;
}

int solveMazeUtil(Maze *maze, Coordinate *path, int x, int y, int index) {
    if (x == maze->end.x && y == maze->end.y) {
        path[index].x = x;
        path[index].y = y;
        return 1;
    }

    if (isValidMove(maze, x, y)) {
        path[index].x = x;
        path[index].y = y;

        if (solveMazeUtil(maze, path, x + 1, y, index + 1)) {
            return 1;
        }

        if (solveMazeUtil(maze, path, x, y + 1, index + 1)) {
            return 1;
        }

        if (solveMazeUtil(maze, path, x - 1, y, index + 1)) {
            return 1;
        }

        if (solveMazeUtil(maze, path, x, y - 1, index + 1)) {
            return 1;
        }

        path[index].x = -1;
        path[index].y = -1;
    }

    return 0;
}

void solveMaze(Maze *maze) {
    Coordinate path[MAX_WIDTH * MAX_HEIGHT];
    int i, j;

    for (i = 0; i < maze->height; i++) {
        for (j = 0; j < maze->width; j++) {
            if (maze->maze[i][j] == 'X') {
                maze->start.x = i;
                maze->start.y = j;
            } else if (maze->maze[i][j] == 0) {
                maze->end.x = i;
                maze->end.y = j;
            }
        }
    }

    if (solveMazeUtil(maze, path, maze->start.x, maze->start.y, 0)) {
        printPath(path, maze->width * maze->height);
    } else {
        printf("SEM_SAIDA\n");
    }
}

int main() {
    int numMazes, i;
    Maze mazes[MAX_WIDTH * MAX_HEIGHT];

    scanf("%d", &numMazes);

    for (i = 0; i < numMazes; i++) {
        readMaze(&mazes[i]);
        solveMaze(&mazes[i]);
    }

    return 0;
}

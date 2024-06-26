#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WIDTH 100
#define MAX_HEIGHT 100

FILE *input;
FILE *output;

typedef struct {
    int x;
    int y;
    char move[2];
} Coordinate;

typedef struct {
    Coordinate start;
    Coordinate end;
    int width;
    int height;
    char maze[MAX_HEIGHT][MAX_WIDTH];
    int visited[MAX_HEIGHT][MAX_WIDTH];
} Maze;

void readMaze(Maze *maze, FILE *input) {
    int i, j;
    char temp;
    fscanf(input, "%d %d", &maze->width, &maze->height);
    for (i = 0; i < maze->height; i++) {
        for (j = 0; j < maze->width; j++) {            
            fscanf(input, "%c", &temp);
            if (temp == '\n' || temp == ' ' || temp == EOF) {
                j--;
                continue;
            }
            maze->maze[i][j] = temp;            
            if (maze->maze[i][j] == 'X') {
                maze->start.x = i;
                maze->start.y = j;
            }
        }
    }
}


int isValidMove(Maze *maze, int x, int y) {
    return (x >= 0 && x < maze->height) &&
        (y >= 0 && y < maze->width) &&
        (maze->maze[x][y] != '1') &&
        (maze->visited[x][y] == 0);
}


int solveMazeUtilIt(Maze *maze, Coordinate *path, int x, int y, int index, char move[2]) {
    while(x != maze->end.x || y != maze->end.y){
        maze->visited[x][y] = 1; // Mark the current cell as visited
        path[index].x = x;
        path[index].y = y;

        if (isValidMove(maze, x, y + 1) && !maze->visited[x][y + 1]) {
            y++;
            move = "D";
            strcpy(path[index].move, move);
            index++;
            path[index].x = x;
            path[index].y = y;
            fprintf(output, "%s@%d,%d->%d,%d\n", move, path[index-1].x, path[index-1].y, x, y);
            maze->visited[x][y] = 1;
        } else if (isValidMove(maze, x - 1, y) && !maze->visited[x - 1][y]) {
            x--;
            move = "F";
            strcpy(path[index].move, move);
            index++;
            path[index].x = x;
            path[index].y = y;
            fprintf(output, "%s@%d,%d->%d,%d\n", move, path[index-1].x, path[index-1].y, x, y);
            maze->visited[x][y] = 1;
        } else if (isValidMove(maze, x, y - 1) && !maze->visited[x][y - 1]) {
            y--;
            move = "E";
            strcpy(path[index].move, move);
            index++;
            path[index].x = x;
            path[index].y = y;
            fprintf(output, "%s@%d,%d->%d,%d\n", move, path[index-1].x, path[index-1].y, x, y);
            maze->visited[x][y] = 1;
        } else if (isValidMove(maze, x + 1, y) && !maze->visited[x + 1][y]) {
            x++;
            move = "T";
            strcpy(path[index].move, move);
            index++;
            path[index].x = x;
            path[index].y = y;
            fprintf(output, "%s@%d,%d->%d,%d\n", move, path[index-1].x, path[index-1].y, x, y);
            maze->visited[x][y] = 1;
        } else {
            strcpy(path[index].move, "BT");
            index--;
            if (index < 0) {
                return 0;
            }
            fprintf(output, "BT@%d,%d<-%d,%d\n", path[index].x, path[index].y, x, y);
            x = path[index].x;
            y = path[index].y;
        }
    }
    fprintf(output, "%s@%d,%d->%d,%d\n", path[index].move, path[index].x, path[index].y, x, y);
    path[index].x = x;
    path[index].y = y;
    fprintf(output, "SAIDA@%d,%d\n", x, y);    
    return 1;
}

void solveMaze(Maze *maze) {
    Coordinate path[MAX_HEIGHT * MAX_WIDTH];
    int i, j;
    for (i = 0; i < maze->height; i++) {
        for (j = 0; j < maze->width; j++) {
            if (maze->maze[i][j] == 'X') {
                maze->start.x = i;
                maze->start.y = j;
            } else if ((i == 0 || i == maze->height-1 || j == 0 || j==maze->width-1)
                        && maze->maze[i][j] == '0' && (i != maze->start.x || j != maze->start.y)) {
                maze->end.x = i;
                maze->end.y = j;
            }
        }
    }

    fprintf(output, "INICIO@%d,%d\n", maze->start.x, maze->start.y);

    if (solveMazeUtilIt(maze, path, maze->start.x, maze->start.y, 0, "\0")) {
        return;
    } else {
        fprintf(output, "SEM_SAIDA\n");
    }
}

int main(int argc, char *argv[]) {
    clock_t begin = clock();
    clock_t mid1, end;   
    printf("clock_t = %ld\n", begin);
    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    
    int numMazes, i;
    

    fscanf(input, "%d", &numMazes);
    printf("Numero de labirintos: %d\n", numMazes);
    Maze mazes[numMazes];   
    mid1 = clock();
    for (i = 0; i < numMazes; i++) {
        fprintf(output, "L%d:\n", i);
        readMaze(&mazes[i], input);
        solveMaze(&mazes[i]);
    }
    end = clock();    
    printf("Tempo de execucao: %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
    printf("Tempo de execucao de leitura: %f\n", (double)(mid1 - begin) / CLOCKS_PER_SEC);
    printf("Tempo de execucao de resolucao: %f\n", (double)(end - mid1) / CLOCKS_PER_SEC);
    return 0;
}

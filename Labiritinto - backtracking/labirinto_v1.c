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
    printf("Labirinto %d x %d\n", maze->width, maze->height);
    for (i = 0; i < maze->height; i++) {
        for (j = 0; j < maze->width; j++) {            
            fscanf(input, "%c", &temp);
            if (temp == '\n' || temp == ' ' || temp == EOF) {
                j--;
                continue;
            }
            maze->maze[i][j] = temp;            
            printf("%c ", maze->maze[i][j]);
            if (maze->maze[i][j] == 'X') {
                maze->start.x = i;
                maze->start.y = j;
            }
        }
        printf("\n");
    }
}

void printPath(Coordinate *path, int length) {
    int i;
    printf("INICIO@%d,%d\n", path[0].x, path[0].y);
    for (i = 1; i < length; i++) {        
        if(strcmp(path[i].move, "S") == 0){
            printf("SAIDA@%d,%d\n", path[i].x, path[i].y);
            break;
        }
        if(strcmp(path[i].move, "BT") == 0){
            printf("BT@%d,%d <- %d,%d\n", path[i].x, path[i].y, path[i-1].x, path[i-1].y);
            break;
        }
        printf("%s@%d,%d -> %d,%d\n", path[i].move, path[i-1].x, path[i-1].y, path[i].x, path[i].y);
        // printf("%s@%d,%d", i == 0 ? "INICIO" : i == length - 1 ? "SAIDA" : "", path[i].x, path[i].y);
        // if (i < length - 1) {
        //     printf(" -> ");
        // }
    }
    printf("\n");
}

int isValidMove(Maze *maze, int x, int y) {
    return (x >= 0 && x < maze->height) &&
        (y >= 0 && y < maze->width) &&
        (maze->maze[x][y] != '1') &&
        (maze->visited[x][y] == 0);
}

int solveMazeUtil(Maze *maze, Coordinate *path, int x, int y, int index, char move[2]) {
    //printf("%s\n", move);
    strcpy(path[index-1].move, move);
    if (x == maze->end.x && y == maze->end.y) {
        printf("%s@%d,%d->%d,%d\n", path[index].move, path[index].x, path[index].y, x, y);
        fprintf(output, "%s@%d,%d->%d,%d\n", path[index].move, path[index].x, path[index].y, x, y);
        path[index].x = x;
        path[index].y = y;
        //printf("%s\n", move);
        strcpy(path[index].move, move);
        //printf("path[%d].move = %s@%d,%d -> %d,%d\n", index, path[index].move, path[index-1].x, path[index-1].y, path[index].x, path[index].y);
        index++;
        printf("SAIDA@%d,%d\n", x, y);
        fprintf(output, "SAIDA@%d,%d\n", x, y);
        return 1;
    }
    //printf("maze[%d][%d] e valido? %d\n", x, y, isValidMove(maze, x, y));
    if (isValidMove(maze, x, y) && !maze->visited[x][y]) {
        //printf("%s\n", move);
        strcpy(path[index].move, move);
        if (index != 0){
            printf("%s@%d,%d->%d,%d\n", path[index].move, path[index].x, path[index].y, x, y);
            fprintf(output, "%s@%d,%d->%d,%d\n", path[index].move, path[index].x, path[index].y, x, y);
        }
        
        index++;
        maze->visited[x][y] = 1; // Mark the current cell as visited
        path[index].x = x;
        path[index].y = y;

        //printf("Moveu para direita\n");
        if (solveMazeUtil(maze, path, x, y + 1, index, "D")) {            
            //strcpy(path[index].move, "D");
            return 1;
        }

        //printf("Moveu para frente\n");
        if (solveMazeUtil(maze, path, x - 1, y, index, "F")) {
            //strcpy(path[index].move, "F");
            return 1;
        }

        //printf("Moveu para esquerda\n");
        if (solveMazeUtil(maze, path, x, y - 1, index, "E")) {
            //strcpy(path[index].move, "E");
            return 1;
        }

        //printf("Moveu para tras\n");
        if (solveMazeUtil(maze, path, x + 1, y, index, "T")) {
            //strcpy(path[index].move, "T");
            return 1;
        }

        //maze->visited[x][y] = 0; // Unmark the current cell as visited
        //printf("Backtracking\n");
        //printf("Index: %d\n", index);
        if (index != 1) {
            strcpy(path[index].move, "BT");
            printf("%s@%d,%d<-%d,%d\n", path[index].move, path[index-1].x, path[index-1].y, x, y);
            fprintf(output, "%s@%d,%d<-%d,%d\n", path[index].move, path[index-1].x, path[index-1].y, x, y);
        }
        
        // strcpy(path[index].move, "BT");
        // printf("%s@%d,%d<-%d,%d\n", path[index].move, path[index-1].x, path[index-1].y,path[index].x, path[index].y);
        
        //printf("path[%d].move = %s\n", index, path[index].move);
        index++;
        //path[index].x = -1;
        //path[index].y = -1;
    }

    return 0;
}

void solveMaze(Maze *maze) {
    Coordinate path[100];
    int i, j;
    int visited[maze->height][maze->width];
    printf("Altura: %d x Largura: %d\n", maze->height, maze->width);
    for (i = 0; i < maze->height; i++) {
        //printf("Linha %d: \n", i);
        for (j = 0; j < maze->width; j++) {
            // printf("Coluna %d: ", j);
            // printf("%c \n", maze->maze[i][j]);
            if (maze->maze[i][j] == 'X') {
                printf("Encontrou o inicio em %d, %d\n", i, j);
                maze->start.x = i;
                maze->start.y = j;
            } else if ((i == 0 || i == maze->height-1 || j == 0 || j==maze->width-1)
                        && maze->maze[i][j] == '0' && (i != maze->start.x || j != maze->start.y)) {
                maze->end.x = i;
                maze->end.y = j;
                printf("Encontrou a saida em %d, %d\n", i, j);
            }
        }
    }

    printf("INICIO@%d,%d\n", maze->start.x, maze->start.y);
    fprintf(output, "INICIO@%d,%d\n", maze->start.x, maze->start.y);

    if (solveMazeUtil(maze, path, maze->start.x, maze->start.y, 0, "\0")) {
        printPath(path, maze->width * maze->height);
    } else {
        printf("SEM_SAIDA\n");
        fprintf(output, "SEM_SAIDA\n");
    }
}

int main(int argc, char *argv[]) {
    clock_t begin = clock();
    printf("clock_t = %ld\n", begin);
    // Ilustrando uso de argumentos de programa
    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
    // Abrindo arquivos
    // FILE *input = fopen(argv[1], "r");
    // FILE *output = fopen(argv[2], "w");
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    
    int numMazes, i;
    

    fscanf(input, "%d", &numMazes);
    printf("Numero de labirintos: %d\n", numMazes);
    Maze mazes[numMazes];   

    for (i = 0; i < numMazes; i++) {
        fprintf(output, "L%d:\n", i);
        readMaze(&mazes[i], input);
        solveMaze(&mazes[i]);
    }

    return 0;
}

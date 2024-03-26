#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

FILE *input;
FILE *output;

int main(int argc, char *argv[]) {
    printf("#ARGS = %i\n", argc);
    printf("PROGRAMA = %s\n", argv[0]);
    printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    char c;
    while ((c = fgetc(input)) != EOF) {
        fputc(c, output);
    }

    fclose(input);
    fclose(output);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FLOAT_TO_INT(x) ((x) >= 0 ? (int)((x) + 0.5) : (int)((x)-0.5))

int main(int argc, char *argv[]) {
  clock_t begin = clock();
  printf("Hello World\n");
  // Ilustrando uso de argumentos de programa
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
  // Abrindo arquivos
  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  int n;
  fscanf(input, "%d", &n);
  printf("n = %d\n", n);
  char **containers = (char **)malloc(sizeof(char *) * n);
  char **cnpjs = (char **)malloc(sizeof(char *) * n);
  int *weights = (int *)malloc(sizeof(int) * n);

  // Cadastro de containers
  for (int i = 0; i < n; i++) {
    containers[i] = malloc(sizeof(char) * 11);
    cnpjs[i] = malloc(sizeof(char) * 18);
    fscanf(input, "%s %s %d", containers[i], cnpjs[i], &weights[i]);
    // printf("Cadastrado: %s %s %d\n", containers[i], cnpjs[i], weights[i]);
    //  fprintf(output, "%s %s %d\n", containers[i], cnpjs[i], weights[i]);
  }

  // Cadastro de inspeção
  int m;
  fscanf(input, "%d", &m);
  printf("m = %d\n", m);
  char **icontainers = (char **)malloc(sizeof(char *) * m);
  char **icnpjs = (char **)malloc(sizeof(char *) * m);
  int *iweights = (int *)malloc(sizeof(int) * m);

  for (int i = 0; i < m; i++) {
    icontainers[i] = malloc(sizeof(char) * 11);
    icnpjs[i] = malloc(sizeof(char) * 18);
    fscanf(input, "%s %s %d", icontainers[i], icnpjs[i], &iweights[i]);
  }

  char **cnpjresult = (char **)malloc(sizeof(char *) * m);
  char **weightresult = (char **)malloc(sizeof(int) * m);
  int cnpjcount = 0;
  int weightcount = 0;
  // Inspeção de CNPJ e Peso
  int weightdiff;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if ((char)containers[i][0] == (char)icontainers[j][0]) {
        if (strcmp(containers[i], icontainers[j]) == 0) {

          if (strcmp(cnpjs[i], icnpjs[j]) != 0) {
            // printf("Entrei cnpj\n");
            cnpjresult[cnpjcount] = malloc(sizeof(char) * 54);
            sprintf(cnpjresult[cnpjcount], "%s: %s<->%s", containers[i],
                    cnpjs[i], icnpjs[j]);
            cnpjcount++;
            // printf("CNPJ: %s\n", cnpjresult[cnpjcount - 1]);
            break;
          }

          weightdiff = abs(weights[i] - iweights[j]);
          double weightperc = (double)weightdiff / weights[i];

          if (weightperc > 0.1) {
            // printf("Entrei peso\n");
            weightresult[weightcount] = malloc(sizeof(char) * 27);
            sprintf(weightresult[weightcount], "%s: %dkg (%d%%)", containers[i],
                    weightdiff, FLOAT_TO_INT(weightperc * 100));
            weightcount++;
            // printf("Peso: %s\n", weightresult[weightcount - 1]);
            break;
          }
        }
      }
    }
  }
  clock_t mid = clock();
  for (int i = 0; i < cnpjcount; i++) {
    fprintf(output, "%s\n", cnpjresult[i]);
  }

  for (int i = 0; i < weightcount; i++) {
    fprintf(output, "%s\n", weightresult[i]);
  }
  clock_t end = clock();
  printf("Time-mid: %f\nTime-end: %f\nEnd-Mid: %f\n",
         ((double)(mid - begin) / CLOCKS_PER_SEC),
         ((double)(end - begin) / CLOCKS_PER_SEC),
         ((double)(end - mid) / CLOCKS_PER_SEC));

  free(containers);
  free(cnpjs);
  free(weights);
  free(icontainers);
  free(icnpjs);
  free(iweights);

  fclose(input);
  fclose(output);
  return 0;
}

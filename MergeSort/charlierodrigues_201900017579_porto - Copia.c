#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define FLOAT_TO_INT(x) ((x) >= 0 ? (int)((x) + 0.5) : (int)((x)-0.5))

void copiar(char **destino, char **origem, int quantidade) {
  for (int i = 0; i < quantidade; i++) {
    //printf("%s agora é %s\n", destino[i], origem[i]);
    destino[i] = origem[i];
  }
}
/*
void copiar2(char **destino, char **origem, int quantidade) {
  for (int i = 0; i < quantidade; i++) {
    destino[i] = origem[i];
  }
}
*/
void intercalar(char **S, char **E, int i, int m, int j, char **cont) {
  int i1 = i, i2 = m + 1, k = i, t, u = 0;
  int weightE1, weightE2;
  char* num1 = malloc(sizeof(char)*5);
  char* num2 = malloc(sizeof(char)*5);
  //printf("Masqueloucura...\n");
  while (i1 <= m && i2 <= j) {
    t = 20;
    u = 0;
    while(E[i1][t]!='('){ t++;}
    while(E[i1][t+1]!=37){
        //printf("E[i1][t]=%c ",E[i1][t+1]);
        num1[u] = E[i1][t+1];
        t++;
        u++;
    }
    t = 20;
    u = 0;
    while(E[i2][t]!='(') {t++;}
    while(E[i2][t+1]!=37){
        //printf("E[i2][t]=%d ",E[i2][t+1]);
        num2[u] = E[i2][t+1];
        t++;
        u++;
    }
    weightE1 = atoi(num1);
    weightE2 = atoi(num2);
    //printf("Comparando %d e %d...\n", weightE1, weightE2);
    if (weightE1 <= weightE2) {
      /*aux = cont[i1];
      cont[i1] = cont[k];
      cont[k] = aux;
      */
      S[k++] = E[i1++];
    } else {
      /*aux = cont[i2];
      cont[i2] = cont[k];
      cont[k] = aux;
      */
      S[k++] = E[i2++];
    }
  }
  if (i1 > m){
    copiar(&S[k], &E[i2], j - i2 + 1);
    //copiar(&cont[k], &aux[i2], j - i2 + 1);
  }
  else{
    copiar(&S[k], &E[i1], m - i1 + 1);
    //copiar2(&cont[k], &aux[i1], m - i1 + 1);
  }
  copiar(&E[i], &S[i], j - i + 1);
  //copiar2(&cont[i], &aux[i], j - i + 1);
}

void mergesort(char **S, char **E, int i, int j, char **cont) {
  if (i < j) {
    int m = i + (j - i) / 2;
    mergesort(S, E, i, m, cont);
    mergesort(S, E, m + 1, j, cont);
    intercalar(S, E, i, m, j, cont);
  }
}

int main(int argc, char *argv[]) {
  clock_t begin = clock();
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
  char **containerinspection = (char **)malloc(sizeof(char *) * m);
  double *weightinspection = (double *)malloc(sizeof(double) * m);
  int cnpjcount = 0;
  int weightcount = 0;
  int weightdiff;

  // Inspeção
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      // Comparar o primeiro caractere do container cadastrado e a inspecionar
      if ((char)containers[i][0] == (char)icontainers[j][0]) {
        // Comparar a string inteira caso o primeiro caractere seja igual
        if (strcmp(containers[i], icontainers[j]) == 0) {
          // Comparar o CNPJ cadastrado e a inspecionar
          if (strcmp(cnpjs[i], icnpjs[j]) != 0) {
            cnpjresult[cnpjcount] = malloc(sizeof(char) * 54);
            sprintf(cnpjresult[cnpjcount], "%s: %s<->%s", containers[i],
                    cnpjs[i], icnpjs[j]);
            cnpjcount++;
            break;
          }

          weightdiff = abs(weights[i] - iweights[j]);
          double weightperc = ((double)weightdiff / weights[i])*100;
          // Comparar o peso cadastrado e a inspecionar
          if (weightperc >= 10.5) {
            containerinspection[weightcount] = malloc(sizeof(char) * 24);
            weightinspection[weightcount] = weightperc;
            sprintf(containerinspection[weightcount], "%s: %dkg (%d%%)", containers[i],
                    weightdiff, FLOAT_TO_INT(weightperc));
            weightcount++;
            break;
          }
        }
      }
    }
  }

  clock_t mid = clock();
  //double *weightsorted = (double *)malloc(sizeof(double) * weightcount);
  char **weightsorted = (char **)malloc(sizeof(char *) * weightcount);

  // Ordenação da inspeção por peso em ordem decrescente
  //mergesort(weightsorted, weightinspection, 0, weightcount - 1, containerinspection);
  mergesort(weightsorted, containerinspection, 0, weightcount - 1, containerinspection);

  // Escrita do resultado
  for (int i = 0; i < cnpjcount; i++) {
    fprintf(output, "%s\n", cnpjresult[i]);
  }
  for (int i = weightcount - 1; i >= 0; i--) {
    //fprintf(output, "%s (%d%%)\n", containerinspection[i], FLOAT_TO_INT(weightsorted[i] * 100));
    fprintf(output, "%s\n", weightsorted[i]);
  }

  clock_t end = clock();
  printf("Time-mid: %f\nTime-end: %f\nEnd-Mid: %f\n",
         ((double)(mid - begin) / CLOCKS_PER_SEC),
         ((double)(end - begin) / CLOCKS_PER_SEC),
         ((double)(end - mid) / CLOCKS_PER_SEC));

  free(containerinspection);
  free(cnpjresult);
  free(icontainers);
  free(icnpjs);
  free(containers);
  free(cnpjs);
  free(weightsorted);
  free(weights);
  free(iweights);
  free(weightinspection);

  fclose(input);
  fclose(output);
  return 0;
}

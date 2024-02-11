#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar um pacote
typedef struct {
  char code[14];
  int price;
  int weight;
  int volume;
} Package;

// Estrutura para representar um veículo
typedef struct {
  char plate[8];
  int max_weight;
  int max_volume;
  double total_price;
  int total_weight;
  int total_volume;
  Package *packages;
  int num_packages;
} Vehicle;
/*
void findItems(int table[][W], Package packages[], int i, int j) {
  if (i == 0 || j == 0) {
    return;
  }
  if (table[i][j] != table[i - 1][j]) {
    printf("Item compõe o preço final: weight = %d, price = %d\n",
           packages[i - 1].weight, packages[i - 1].price);
    findItems(table, packages, i - 1, j - packages[i - 1].weight);
  } else {
    findItems(table, packages, i - 1, j);
  }
}
*/
int **dynamicSorting(Vehicle *vehicle, Package *packages, int num_packages) {
  int W = vehicle->max_weight;
  int **table = (int **)malloc((num_packages + 1) * (W + 1) * sizeof(int *));

  // Inicializando tabela
  for (int i = 0; i < num_packages; i++) {
    table[i][0] = 0;
  }

  for (int j = 0; j < W; j++) {
    table[0][j] = 0;
  }

  // Preenchendo tabela
  for (int i = 0; i < num_packages; i++) {
    for (int j = 0; j < W; j++) {
      // Caso o pacote não caiba no subcontainer
      printf("packages[%d].weight = %d, W= %d\n", i, packages[i].weight, j + 1);
      if (packages[i].weight > j + 1) {
        table[i + 1][j + 1] = table[i][j + 1];
        printf("<package[%d].price = %d\n", i, packages[i].price);
        printf("<table[%d][%d] = %d\n\n", i + 1, j + 1, table[i][j + 1]);
        // Caso o pacote caiba no subcontainer
      } else {
        // table[i + 1][j + 1] = table[i][j - packages[i].weight + 1] +
        // packages[i].price;
        printf("table[%d][%d] = %d > %d?\n", i, j - packages[i].weight + 1,
               table[i][j - packages[i].weight + 1], table[i][j]);
        (table[i][j - packages[i].weight + 1] + packages[i].price >
         table[i][j + 1])
            ? (table[i + 1][j + 1] =
                   table[i][j - packages[i].weight + 1] + packages[i].price)
            : (table[i + 1][j + 1] = table[i][j + 1]);
        printf(">=package[%d].price = %d\n", i, packages[i].price);
        printf(">=table[%d][%d] = %d\n\n", i + 1, j + 1, table[i + 1][j + 1]);
      }
    }
  }

  // findItems(table, packages, num_packages, W);

  return table;
}

int main(int argc, char *argv[]) {
  // Ilustrando uso de argumentos de programa
  printf("#ARGS = %i\n", argc);
  printf("PROGRAMA = %s\n", argv[0]);
  printf("ARG1 = %s, ARG2 = %s\n", argv[1], argv[2]);
  // Abrindo arquivos
  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");

  // Veículos
  int num_vehicles;
  fscanf(input, "%d", &num_vehicles);
  /*
  char **plate;
  int max_load[num_vehicles];
  int max_vol[num_vehicles];
  */
  printf("Numero de veiculos: %d\n", num_vehicles);

  Vehicle *vehicles = malloc(sizeof(Vehicle) * num_vehicles);

  // plate = (char **)malloc(num_vehicles * sizeof(char *));
  for (int i = 0; i < num_vehicles; i++) {
    // plate[i] = malloc(sizeof(char) * 7);
    // fscanf(input, "%s %d %d", plate[i], &max_load[i], &max_vol[i]);
    fscanf(input, "%s %d %d", vehicles[i].plate, &vehicles[i].max_weight,
           &vehicles[i].max_volume);
  }

  // Pacotes
  int num_packages;
  fscanf(input, "%d", &num_packages);
  /*
  char **code;
  double price[num_packages];
  int weight[num_packages];
  int vol[num_packages];
  */
  printf("Numero de pacotes: %d\n", num_packages);

  Package *packages = malloc(sizeof(Package) * num_packages);

  // code = (char **)malloc(num_packages * sizeof(char *));
  for (int i = 0; i < num_packages; i++) {
    // code[i] = malloc(sizeof(char) * 13);
    // fscanf(input, "%s %lf %d %d", code[i], &price[i], &weight[i], &vol[i]);
    double price;
    fscanf(input, "%s %lf %d %d", packages[i].code, &price, &packages[i].weight,
           &packages[i].volume);
    packages[i].price = (int)(price * 100);
  }

  // Escrever na saída
  // for (int i = 0; i < num_vehicles; i++) {
  //   printf("%s %d %d\n", plate[i], max_load[i], max_vol[i]);
  //   fprintf(output, "%s %d %d\n", plate[i], max_load[i], max_vol[i]);
  //   free(plate[i]);
  // }
  for (int i = 0; i < num_vehicles; i++) {
    printf("%s %d %d\n", vehicles[i].plate, vehicles[i].max_weight,
           vehicles[i].max_volume);
    fprintf(output, "%s %d %d\n", vehicles[i].plate, vehicles[i].max_weight,
            vehicles[i].max_volume);
  }

  // for (int i = 0; i < num_packages; i++) {
  //   printf("%s %.2lf %d %d\n", code[i], price[i], weight[i], vol[i]);
  //   fprintf(output, "%s R$%.2lf %dKg %dL\n", code[i], price[i], weight[i],
  //           vol[i]);
  //   free(code[i]);
  // }
  for (int i = 0; i < num_packages; i++) {
    printf("%s %.2lf %d %d\n", packages[i].code,
           (double)packages[i].price / 100, packages[i].weight,
           packages[i].volume);
    fprintf(output, "%s R$%.2lf %dKg %dL\n", packages[i].code,
            (double)packages[i].price / 100, packages[i].weight,
            packages[i].volume);
  }
  int **table = (int **)malloc((num_packages + 1) *
                               (vehicles[0].max_weight + 1) * sizeof(int *));
  table = dynamicSorting(&vehicles[0], packages, num_packages);
  printf("Resultado 0: R$%.2f\n",
         (float)(table[num_packages][vehicles[0].max_weight] / 100));
  // printf("Resultado 1: R$%.2f\n", (float)(dynamicSorting(&vehicles[1],
  // packages, num_packages) / 100));

  /*
  int current_capacity;
  int num_cars;
  */
  // free(plate);
  // free(code);

  free(vehicles);
  free(packages);

  fclose(input);
  fclose(output);
  return 0;
}
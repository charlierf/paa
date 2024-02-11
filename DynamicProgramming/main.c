#include <stdio.h>
#include <stdlib.h>

// Estrutura para representar um pacote
typedef struct {
  char code[14];
  long int price;
  int weight;
  int volume;
} Package;

// Estrutura para representar um veículo
typedef struct {
  char plate[8];
  int max_weight;
  int max_volume;
  long int total_price;
  int total_weight;
  int total_volume;
  Package *packages;
  int num_packages;
} Vehicle;

// Função para calcular o valor total do frete para um veículo
long int dynamicSorting(Vehicle *vehicle, Package *packages, int num_packages) {
   int W = vehicle->max_weight;
   int V = vehicle->max_volume;
  // Alocar dinamicamente a tabela
  long int ***table = malloc((num_packages + 1) * sizeof(long int **));
  for (int i = 0; i <= num_packages; i++) {
    table[i] = malloc((W + 1) * sizeof(long int *));
    for (int j = 0; j <= W; j++) {
      table[i][j] = malloc((V + 1) * sizeof(long int));
    }
  }
  // printf("Placa: %s\n", vehicle->plate);

  // Inicializando tabela
  // for (int i = 0; i <= num_packages; i++) {
  //   for (int j = 0; j <= W; j++) {
  //     for (int k = 0; k <= V; k++) {
  //       table[i][j][k] = 0;
  //     }
  //   }
  // }


  // Preenchendo tabela
  for (int i = 0; i < num_packages; i++) {
    for (int j = 0; j < W; j++) {
      for (int k = 0; k < V; k++) {
        if (i == 0 || j == 0 || k == 0)
          table[i][j][k] = 0;
        // Caso o pacote não caiba no subcontainer
        if (packages[i].weight > j || packages[i].volume > k) {
          table[i + 1][j + 1][k + 1] = table[i][j + 1][k + 1];
          // printf("table[%d][%d][%d] = %ld\n", i + 1, j + 1, k + 1,
          //        table[i][j + 1][k + 1]);
        } else {
          // Caso o pacote caiba no subcontainer
          long int value_with_package =
              table[i][j - packages[i].weight + 1][k - packages[i].volume + 1] +
              packages[i].price;
          long int value_without_package = table[i][j + 1][k + 1];
          table[i + 1][j + 1][k + 1] =
              (value_with_package > value_without_package)
                  ? value_with_package
                  : value_without_package;
          // printf("table[%d][%d][%d] = %ld\n", i + 1, j + 1, k + 1,
          //        table[i + 1][j + 1][k + 1]);
        }
      }
    }
  }

  // Imprimir os pacotes que foram incluídos no cálculo do valor total
  printf("\nPACOTES INCLUIDOS\n");
  int weight_left = W;
  int volume_left = V;
  for (int i = num_packages; i > 0 && table[i][weight_left][volume_left] > 0;
       i--) {
    if (table[i][weight_left][volume_left] !=
        table[i - 1][weight_left][volume_left]) {
      printf("%s\n", packages[i - 1].code);
      weight_left -= packages[i - 1].weight;
      volume_left -= packages[i - 1].volume;
    }
  }
/*
  // Liberar memória alocada para a tabela
  for (int i = 0; i <= num_packages; i++) {
    for (int j = 0; j <= W; j++) {
      free(table[i][j]);
    }
    free(table[i]);
  }
  free(table);
*/
  return table[num_packages][W][V];
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
    packages[i].price = (long int)(price * 100);
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
  /*int W = vehicles[1].max_weight;
  int V = vehicles[1].max_volume;
  long int resultado = dynamicSorting(W, V, packages, num_packages);
  */
  //printf("Resultado 0: R$%.2lf\n", (double)resultado / 100);
   printf("Resultado 1: R$%.2f\n", (double)dynamicSorting(&vehicles[1],
   packages, num_packages) / 100);

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

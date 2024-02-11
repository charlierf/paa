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

long int dynamicSorting(Vehicle *vehicle, Package *packages, int num_packages) {
  int W = vehicle->max_weight;
  long int table[num_packages + 1][W + 1];

  // Inicializando tabela
  for (int i = 0; i <= num_packages; i++) {
    for (int j = 0; j <= W; j++) {
      table[i][j] = 0;
    }
  }

  // Preenchendo tabela
  for (int i = 0; i < num_packages; i++) {
    for (int j = 0; j < W; j++) {
      // Caso o pacote não caiba no subcontainer
      if (packages[i].weight > j + 1) {
        table[i + 1][j + 1] = table[i][j + 1];

        // Caso o pacote caiba no subcontainer
      } else {
        long int value_with_package =
            table[i][j - packages[i].weight + 1] + packages[i].price;
        long int value_without_package = table[i][j + 1];
        table[i + 1][j + 1] = value_with_package > value_without_package
                                  ? value_with_package
                                  : value_without_package;
      }
    }
  }

  printf("\nPACOTES INCLUÍDOS\n");
  int weight_left = W;
  for (int i = num_packages; i > 0 && table[i][weight_left] > 0; i--) {
    if (table[i][weight_left] != table[i - 1][weight_left]) {
      printf("%s\n", packages[i - 1].code);
      weight_left -= packages[i - 1].weight;
    }
  }

  return table[num_packages][W];
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
  printf("Numero de veiculos: %d\n", num_vehicles);

  Vehicle *vehicles = malloc(sizeof(Vehicle) * num_vehicles);

  for (int i = 0; i < num_vehicles; i++) {
    fscanf(input, "%s %d %d", vehicles[i].plate, &vehicles[i].max_weight,
           &vehicles[i].max_volume);
  }

  // Pacotes
  int num_packages;
  fscanf(input, "%d", &num_packages);
  printf("Numero de pacotes: %d\n", num_packages);

  Package *packages = malloc(sizeof(Package) * num_packages);

  for (int i = 0; i < num_packages; i++) {
    double price;
    fscanf(input, "%s %lf %d %d", packages[i].code, &price, &packages[i].weight,
           &packages[i].volume);
    packages[i].price = (long int)(price * 100);
  }

  // Escrever na saída
  for (int i = 0; i < num_vehicles; i++) {
    printf("%s %d %d\n", vehicles[i].plate, vehicles[i].max_weight,
           vehicles[i].max_volume);
    fprintf(output, "%s %d %d\n", vehicles[i].plate, vehicles[i].max_weight,
            vehicles[i].max_volume);
  }

  for (int i = 0; i < num_packages; i++) {
    printf("%s %.2lf %d %d\n", packages[i].code,
           (double)packages[i].price / 100, packages[i].weight,
           packages[i].volume);
    fprintf(output, "%s R$%.2lf %dKg %dL\n", packages[i].code,
            (double)packages[i].price / 100, packages[i].weight,
            packages[i].volume);
  }

  printf("Resultado 0: R$%.2lf\n",
         (double)dynamicSorting(&vehicles[1], packages, num_packages) / 100);

  free(vehicles);
  free(packages);

  fclose(input);
  fclose(output);
  return 0;
}
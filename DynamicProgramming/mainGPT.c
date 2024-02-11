#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Função para calcular o valor total do frete para um veículo
void calculateTotalPrice(Vehicle *vehicle) {
  vehicle->total_price = 0;
  for (int i = 0; i < vehicle->num_packages; i++) {
    vehicle->total_price += vehicle->packages[i].price;
  }
}

// Função para imprimir a saída para um veículo
void printOutput(FILE *output, Vehicle *vehicle) {
  fprintf(output, "[%s]R$%.2lf,%dKG(%.0lf%%),%dL(%.0lf%%)\n", vehicle->plate,
          vehicle->total_price, vehicle->total_weight,
          (double)vehicle->total_weight * 100 / vehicle->max_weight,
          vehicle->total_volume,
          (double)vehicle->total_volume * 100 / vehicle->max_volume);
  for (int i = 0; i < vehicle->num_packages; i++) {
    fprintf(output, "%s\n", vehicle->packages[i].code);
  }
}

int dynamicSorting(Vehicle *vehicle, Package *packages, int num_packages) {
  int W = vehicle->max_weight;
  int table[num_packages + 1][W];

  // Inicializando tabela
  for (int i = 0; i < num_packages; i++) {
    for (int j = 0; j < W; j++) {
      table[i][j] = 0;
    }
  }

  // Preenchendo tabela
  for (int i = 1; i < num_packages; i++) {
    for (int j = 1; j < W; j++) {
      // Caso o pacote não caiba no subcontainer
      if (packages[i].weight <= j) {
        table[i][j] = table[i - 1][j];
        // Caso o pacote caiba no subcontainer
      } else {
        table[i][j] = table[i - 1][j - packages[i].weight] + packages[i].price;
      }
    }
  }
  return table[num_packages - 1][W - 1];
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Uso: %s arquivo_entrada arquivo_saida\n", argv[0]);
    return 1;
  }

  FILE *input = fopen(argv[1], "r");
  FILE *output = fopen(argv[2], "w");
  if (input == NULL || output == NULL) {
    printf("Erro ao abrir arquivos de entrada ou saída.\n");
    return 1;
  }

  int num_vehicles;
  fscanf(input, "%d", &num_vehicles);

  Vehicle *vehicles = (Vehicle *)malloc(num_vehicles * sizeof(Vehicle));

  for (int i = 0; i < num_vehicles; i++) {
    fscanf(input, "%s %d %d", vehicles[i].plate, &vehicles[i].max_weight,
           &vehicles[i].max_volume);
    vehicles[i].total_price = 0;
    vehicles[i].total_weight = 0;
    vehicles[i].total_volume = 0;
    vehicles[i].packages = NULL;
    vehicles[i].num_packages = 0;
  }

  int num_packages;
  fscanf(input, "%d", &num_packages);
  Package *packages = (Package *)malloc(num_packages * sizeof(Package));

  for (int i = 0; i < num_packages; i++) {
    double price;
    fscanf(input, "%s %lf %d %d", packages[i].code, &price, &packages[i].weight,
           &packages[i].volume);
    packages[i].price = (int)price;
  }
  /*
    for (int i = 0; i < num_vehicles; i++) {
      for (int j = 0; j < num_packages; j++) {
        if (packages[j].weight <=
                vehicles[i].max_weight - vehicles[i].total_weight &&
            packages[j].volume <=
                vehicles[i].max_volume - vehicles[i].total_volume) {
          vehicles[i].packages = (Package *)realloc(
              vehicles[i].packages,
              (vehicles[i].num_packages + 1) * sizeof(Package));
          vehicles[i].packages[vehicles[i].num_packages++] = packages[j];
          vehicles[i].total_weight += packages[j].weight;
          vehicles[i].total_volume += packages[j].volume;
        }
      }
      calculateTotalPrice(&vehicles[i]);

  printOutput(output, &vehicles[i]);
  if (vehicles[i].packages != NULL) {
    free(vehicles[i].packages);
  }
}
  */

  printf("Resposta %d\n", dynamicSorting(&vehicles[1], packages, num_packages));

  free(vehicles);
  free(packages);
  fclose(input);
  fclose(output);

  return 0;
}
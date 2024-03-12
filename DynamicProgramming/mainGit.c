#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
  char code[14];
  long int price;
  int weight;
  int volume;
} Package;

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
  int V = vehicle->max_volume;
  long int ***table = malloc((num_packages + 1) * sizeof(long int **));
  for (int i = 0; i <= num_packages; i++) {
    table[i] = malloc((W + 1) * sizeof(long int *));
    for (int j = 0; j <= W; j++) {
      table[i][j] = malloc((V + 1) * sizeof(long int));
    }
  }

  for (int i = 0; i <= num_packages; i++) {
    for (int j = 0; j <= W; j++) {
      for (int k = 0; k <= V; k++) {
        if (i == 0 || j == 0 || k == 0)
          table[i][j][k] = 0;
        else if (packages[i - 1].weight <= j && packages[i - 1].volume <= k)
          table[i][j][k] = fmax(
              packages[i - 1].price +
                  table[i - 1][j - packages[i - 1].weight][k - packages[i - 1].volume],
              table[i - 1][j][k]);
        else
          table[i][j][k] = table[i - 1][j][k];
      }
    }
  }

  int weight_left = W;
  int volume_left = V;
  for (int i = num_packages; i > 0 && table[i][weight_left][volume_left] > 0;
       i--) {
    if (table[i][weight_left][volume_left] !=
        table[i - 1][weight_left][volume_left]) {
      weight_left -= packages[i - 1].weight;
      volume_left -= packages[i - 1].volume;
      vehicle->packages[vehicle->num_packages++] = packages[i - 1];
    }
  }

  long int result = table[num_packages][W][V];
  for (int i = 0; i <= num_packages; i++) {
    for (int j = 0; j <= W; j++) {
      free(table[i][j]);
    }
    free(table[i]);
  }
  free(table);
  return result;
}

int main() {
  int num_vehicles;
  scanf("%d", &num_vehicles);
  Vehicle *vehicles = malloc(sizeof(Vehicle) * num_vehicles);
  for (int i = 0; i < num_vehicles; i++) {
    scanf("%s %d %d", vehicles[i].plate, &vehicles[i].max_weight,
          &vehicles[i].max_volume);
    vehicles[i].packages = malloc(sizeof(Package) * 100);
    vehicles[i].num_packages = 0;
  }

  int num_packages;
  scanf("%d", &num_packages);
  Package *packages = malloc(sizeof(Package) * num_packages);
  for (int i = 0; i < num_packages; i++) {
    double price;
    scanf("%s %lf %d %d", packages[i].code, &price, &packages[i].weight,
          &packages[i].volume);
    packages[i].price = (long int)(price * 100);
  }

  for (int i = 0; i < num_vehicles; i++) {
    vehicles[i].total_price =
        dynamicSorting(&vehicles[i], packages, num_packages);
    printf("[%s]R$%.2lf,%dKG,%dL\n", vehicles[i].plate,
           (double)vehicles[i].total_price / 100, vehicles[i].max_weight,
           vehicles[i].max_volume);
    for (int j = 0; j < vehicles[i].num_packages; j++) {
      printf("%s\n", vehicles[i].packages[j].code);
    }
  }

  free(vehicles);
  free(packages);
  return 0;
}
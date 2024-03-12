#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para representar um pacote
typedef struct {
  char code[14];
  int price;
  int weight;
  int volume;
  char sorted;
} Package;

typedef struct PackageList
{
    int package_index;
    struct PackageList *prox;
    struct PackageList *ant;
} PackageList;

void newPackageList(PackageList *p)
{
    p->package_index = 0;
    p->prox = NULL;
    p->ant = NULL;
}

void newPackage(Package *p, char* code, int price, int weight, int volume)
{
    strcpy(p->code, code);
    p->price = price;
    p->weight = weight;
    p->volume = volume;
    p->sorted = 0;
}

void insertPackage(PackageList *p, int index)
{
    PackageList *nova, *aux;
    nova = malloc (sizeof (PackageList));
    nova->package_index = index;
    nova->prox = NULL;
    if (p->prox == NULL)
    {
        p->prox = nova;
    }
    else
    {
        aux = p;
        while(aux->prox != NULL)
        {
            aux = aux->prox;
        }
        aux->prox = nova;
        aux->prox->ant = aux;
    }
}

void removePackage(PackageList *le, int index)
{
    PackageList *p, *aux;
    p = le->prox;
    while ((p != NULL) && (p->package_index != index))
    {
        aux = p;
        p = p->prox;
    }
    (p->ant)->prox = p->prox;
    
    if(p->prox != NULL){
      (p->prox)->ant = p->ant;
    }
}

// Estrutura para representar um veículo
typedef struct {
  char plate[8];
  int max_weight;
  int max_volume;
  int total_price;
  int total_weight;
  int total_volume;
  Package *packages;
  int num_packages;
} Vehicle;

// Função para calcular o valor total do frete para um veículo
void dynamicSorting(Vehicle *vehicle, Package *packages, int num_packages, PackageList *package_list) {
   int W = vehicle->max_weight;
   int V = vehicle->max_volume;
   int ix;
   PackageList *aux;
   aux = package_list->prox;
  // Alocar dinamicamente a tabela
  unsigned int ***table = malloc((num_packages + 2) * (W+2) * (V+2) * sizeof(unsigned int **));
  
  // Preenchendo tabela
  for (int i = 0; i < num_packages + 1; i++) {
    ix = aux->package_index;
    Package current_package = packages[ix];
    table[i] = malloc((W + 2) * sizeof(unsigned int *));
    for (int j = 0; j < W + 1; j++) {
      table[i][j] = malloc((V + 2) * sizeof(unsigned int));
      for (int k = 0; k < V + 1; k++) {
        if (i == 0 || j == 0 || k == 0){
          table[i][j][k] = 0;
          continue;
        }

        // Caso o pacote não caiba no subcontainer
        if (current_package.weight > j || current_package.volume > k){
          table[i][j][k] = table[i - 1][j][k];

        } else {
          // Caso o pacote caiba no subcontainer
          int value_with_package =
              table[i-1][j - current_package.weight][k - current_package.volume] +
              current_package.price;
          int value_without_package = table[i-1][j][k];
          table[i][j][k] =
              (value_with_package > value_without_package)
                  ? value_with_package
                  : value_without_package;
        }
      }
    }    
    if ((aux->prox != NULL) && (i != 0)){
      aux = aux->prox;
    }
  }

  // Imprimir os pacotes que foram incluídos no cálculo do valor total  
  int weight_left = W;
  int volume_left = V;
  vehicle->packages = malloc(sizeof(Package) * num_packages);
  for (int i = num_packages; i > 0 && table[i][weight_left][volume_left] > 0;
       i--) {
    ix = aux->package_index;
    Package current_package = packages[ix];
    if (table[i][weight_left][volume_left] !=
        table[i - 1][weight_left][volume_left]) {      
      vehicle->total_price += current_package.price;
      vehicle->total_weight += current_package.weight;
      vehicle->total_volume += current_package.volume;
      vehicle->num_packages++;
      vehicle->packages[vehicle->num_packages - 1] = current_package;
      weight_left -= current_package.weight;
      volume_left -= current_package.volume;
      packages[i].sorted = 1;
      removePackage(package_list, ix);
    }
    if (aux->ant != NULL){
      aux = aux->ant;
    }
  }
  vehicle->packages = realloc(vehicle->packages, sizeof(Package) * vehicle->num_packages);
   
  // Liberar memória alocada para a tabela
  for (int i = 0; i <= num_packages; i++) {
    for (int j = 0; j <= W; j++) {
      free(table[i][j]);
    }
    free(table[i]);
  }  
  free(table);

  //return result;
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

  // Veículos
  int num_vehicles;
  fscanf(input, "%d", &num_vehicles);

  printf("Numero de veiculos: %d\n", num_vehicles);

  Vehicle *vehicles = malloc(sizeof(Vehicle) * num_vehicles);

  for (int i = 0; i < num_vehicles; i++) {
    fscanf(input, "%s %d %d", vehicles[i].plate, &vehicles[i].max_weight,
           &vehicles[i].max_volume);
      vehicles[i].total_price = 0;
      vehicles[i].total_weight = 0;
      vehicles[i].total_volume = 0;
      vehicles[i].num_packages = 0;
      vehicles[i].packages = malloc(sizeof(Package));
  }

  // Pacotes
  int num_packages;
  fscanf(input, "%d", &num_packages);
  printf("Numero de pacotes: %d\n", num_packages);

  Package *packages = malloc(sizeof(Package) * (num_packages + 1));
  packages[0].code[0] = '\0';
  packages[0].price = 0;
  packages[0].weight = 0;
  packages[0].volume = 0;
  packages[0].sorted = 0;

  PackageList *package_list;
  package_list = malloc(sizeof(PackageList));
  newPackageList(package_list);

  for (int i = 1; i <= num_packages; i++) {
    double price;
    fscanf(input, "%s %lf %d %d", packages[i].code, &price, &packages[i].weight,
           &packages[i].volume);
    packages[i].price = (int)(price * 100);
    packages[i].sorted = 0;
    insertPackage(package_list, i);
  }
  clock_t mid = clock();
  for (int i = 0; i < num_vehicles; i++) {
   dynamicSorting(&vehicles[i], packages, num_packages, package_list);
   num_packages -= vehicles[i].num_packages;
   fprintf(output, "[%s]R$%.2f,%dKG(%d%%),%dL(%d%%)\n", vehicles[i].plate, (double)vehicles[i].total_price / 100.0, 
   vehicles[i].total_weight, (vehicles[i].total_weight * 100) / vehicles[i].max_weight, vehicles[i].total_volume, 
   (vehicles[i].total_volume * 100) / vehicles[i].max_volume);
  
   for (int j = vehicles[i].num_packages - 1; j >= 0; j--){
     fprintf(output, "%s\n", vehicles[i].packages[j].code);
   }
  }
  
  int pending_value = 0;
  int pending_weight = 0;
  int pending_volume = 0;

  PackageList* aux = package_list->prox;
  for (int i = 1; i <= num_packages; i++) {
      Package current_package = packages[aux->package_index];
      pending_value += current_package.price;
      pending_weight += current_package.weight;
      pending_volume += current_package.volume;
      aux = aux->prox;
  }
  fprintf(output, "[PENDENTE]R$%.2f,%dKG,%dL\n", (double)pending_value / 100.0, pending_weight, pending_volume);
  aux = package_list->prox;
  for (int i = 1; i <= num_packages; i++) {
    fprintf(output, "%s\n", packages[aux->package_index].code);
    aux = aux->prox;
  }

  clock_t end = clock();
  printf("Time-mid: %f\nTime-end: %f\nEnd-Mid: %f\n",
         ((double)(mid - begin) / CLOCKS_PER_SEC),
         ((double)(end - begin) / CLOCKS_PER_SEC),
         ((double)(end - mid) / CLOCKS_PER_SEC));

  free(aux);
  free(vehicles);
  free(packages);
  fclose(input);
  fclose(output);
  return 0;
}

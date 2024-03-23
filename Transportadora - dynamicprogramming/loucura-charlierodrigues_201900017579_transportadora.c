#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para representar um pacote
typedef struct {
  char code[14];
  long int price;
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

void newPackage(Package *p, char* code, long int price, int weight, int volume)
{
    //printf("Criando pacote %s\n", code);
    strcpy(p->code, code);
    p->price = price;
    p->weight = weight;
    p->volume = volume;
    p->sorted = 0;
    //printf("Pacote %s criado\n", p->code);
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
    int i = 0;
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
  long int total_price;
  int total_weight;
  int total_volume;
  Package *packages;
  int num_packages;
} Vehicle;

// Função para calcular o valor total do frete para um veículo
void dynamicSorting(Vehicle *vehicle, Package *packages, int num_packages, PackageList *package_list) {
   int W = vehicle->max_weight;
   int V = vehicle->max_volume;
   PackageList *aux = malloc(sizeof(PackageList));
   aux = package_list->prox;
  // Alocar dinamicamente a tabela
  long int ***table = malloc((num_packages + 2) * sizeof(long int **));
  for (int i = 0; i <= num_packages + 1; i++) {
    table[i] = malloc((W + 2) * sizeof(long int *));
    for (int j = 0; j <= W + 1; j++) {
      table[i][j] = malloc((V + 2) * sizeof(long int));
    }
  }
  
  // Preenchendo tabela
  for (int i = 0; i < num_packages + 1; i++) {
    //printf("Verificando pacote %s\n", packages[aux->package_index].code);
    for (int j = 0; j < W + 1; j++) {
      for (int k = 0; k < V + 1; k++) {
        if (i == 0 || j == 0 || k == 0){
          table[i][j][k] = 0;
          continue;
        }

        // Caso o pacote não caiba no subcontainer
        if (packages[aux->package_index].weight > j || packages[aux->package_index].volume > k){
          table[i][j][k] = table[i - 1][j][k];

        } else {

          // Caso o pacote caiba no subcontainer
          long int value_with_package =
              table[i-1][j - packages[aux->package_index].weight][k - packages[aux->package_index].volume] +
              packages[aux->package_index].price;
          long int value_without_package = table[i-1][j][k];
          table[i][j][k] =
              (value_with_package > value_without_package)
                  ? value_with_package
                  : value_without_package;

        }
      }
    }
    //printf("i = %d - Pacote %s verificado", i, packages[aux->package_index].code);
    if ((aux->prox != NULL) && (i != 0)){
      //printf(", proximo: %s\n", packages[aux->prox->package_index].code);
      aux = aux->prox;
    } else {
      //printf(".\n");
    }
  }
  //printf("\nResultado: %ld\n\n", table[num_packages][W][V]);

  // Imprimir os pacotes que foram incluídos no cálculo do valor total  
  int weight_left = W;
  int volume_left = V;
  for (int i = num_packages; i > 0 && table[i][weight_left][volume_left] > 0;
       i--) {
    
    // Skip packages that have already been sorted
    // if (packages[aux->package_index].sorted == 1){
    //   continue;
    // }
    
    if (table[i][weight_left][volume_left] !=
        table[i - 1][weight_left][volume_left]) {
      
      vehicle->total_price += packages[aux->package_index].price;
      vehicle->total_weight += packages[aux->package_index].weight;
      vehicle->total_volume += packages[aux->package_index].volume;
      vehicle->num_packages++;
      vehicle->packages = realloc(vehicle->packages, sizeof(Package) * vehicle->num_packages);
      vehicle->packages[vehicle->num_packages - 1] = packages[aux->package_index];
      weight_left -= packages[aux->package_index].weight;
      volume_left -= packages[aux->package_index].volume;
      packages[i].sorted = 1;
      removePackage(package_list, aux->package_index);
    }
    if (aux->ant != NULL){
      aux = aux->ant;
    } else {
      printf("Nao ha mais pacotes\n");
    }
  }
 

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
    packages[i].price = (long int)(price * 100);
    packages[i].sorted = 0;
    insertPackage(package_list, i);
  }
  //printf("Pacotes inseridos\n");

  for (int i = 0; i < num_vehicles; i++) {
  
   dynamicSorting(&vehicles[i], packages, num_packages, package_list);
   num_packages -= vehicles[i].num_packages;
    //printf("Numero de pacotes restantes: %d\n", num_packages);
   fprintf(output, "[%s]R$%.2f,%dKG(%d%%),%dL(%d%%)\n", vehicles[i].plate, (double)vehicles[i].total_price / 100.0, 
   vehicles[i].total_weight, (vehicles[i].total_weight * 100) / vehicles[i].max_weight, vehicles[i].total_volume, 
   (vehicles[i].total_volume * 100) / vehicles[i].max_volume);
  
   for (int j = vehicles[i].num_packages - 1; j >= 0; j--){
     fprintf(output, "%s\n", vehicles[i].packages[j].code);
   }
  }
  printf("Pacotes ordenados\n");
  
  long int pending_value = 0;
  int pending_weight = 0;
  int pending_volume = 0;

  for (int i = 1; i <= num_packages; i++) {
    if (packages[i].sorted == 0){
      pending_value += packages[i].price;
      pending_weight += packages[i].weight;
      pending_volume += packages[i].volume;
    }
  }
  fprintf(output, "[PENDENTE]R$%.2f,%dKG,%dL\n", (double)pending_value / 100.0, pending_weight, pending_volume);
  PackageList* aux = package_list->prox;
  for (int i = 1; i <= num_packages; i++) {
    fprintf(output, "%s\n", packages[aux->package_index].code);
    aux = aux->prox;
  }

  free(vehicles);
  free(packages);

  fclose(input);
  fclose(output);
  return 0;
}

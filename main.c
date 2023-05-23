#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define INITIAL_SIZE 10

// functie care determina index-ul sensorului pe care se aplica o alta functie
int get_index(char s[]);
// functie care citeste sensorii din fisierul de input
void read_users(FILE *f, user *users, int n);
// functie care sorteaza senzorii in functie de importanta (PMU > TIRE)
void rezolvare(user *users, int *n, FILE *f);
// functia care citeste produsele din stock-ul magazinului
void read_products(FILE *f, product *products, int n);
// functia de exit necesara problemei
void Exit(user *users, int n, int *ok, FILE *f);
// functie care sterge un anumit senzor din vectorul de senzori
void add_product(user *users, product *products, int *n, int current_type);
void delete_product(user *users, product *products, int *n, int index,
                    int current_type);
void show_products(product *products, int n);
void add_to_cart(user *users, int my_index, int nr_produs, product *products);
// functia elibereaza memoria alocata dinamic
void freemem(user *users, int n);

int main(int argc, char const *argv[])
{ // se deschide fisierul de intrare
  FILE *f = fopen(argv[1], "r");
  if (f == NULL)
  {
    printf("Eroarea la creearea fisierului!\n");
    return 1;
  }
  // declaram un int, "n" care reprezinta numarul de useri din fisier
  int n;
  // citim din fisier acest numar
  fread(&n, sizeof(n), 1, f);
  // declaram un vector de senzori
  user *users;
  // alocam dinamic vectorul de senzori
  users = (user *)malloc(sizeof(user) * n);
  // citim senzorii din fisier prin functia definita anterior
  read_users(f, users, n);

  // apelam functia de rezolvare a problemei propriu-zisa
  rezolvare(users, &n, f);
  return 0;
}
int get_index(char s[])
{ // declaram nr, care reprezinta numarul nostru ce se
  // afla in comanda
  /*i este un iterator care ne va ajuta sa ne plimbam prin vectorul de tip
  char*/
  // negativ ne va spune daca in "s" se va intalni semnul "-"
  int nr = 0, i, negativ = 0;
  // parcurgem comanda data de la tastatura
  for (i = 0; i < strlen(s);
       i++)
  { // verificam daca caracterul de pe pozitia curenta este cifra
    if (s[i] >= '0' && s[i] <= '9')
      // in caz afirmativ este adaugata la numarul nostru
      nr = nr * 10 + (s[i] - '0');
    // daca intalnim semnul "-", negativ va deveni 1
    if (s[i] == '-')
      negativ = 1;
  }
  // in cazul in care numarul nostru este negativ, il inmultim cu (-1)
  if (negativ)
    nr = -nr;
  // returnam numarul obtinut
  return nr;
}

void read_users(FILE *f, user *users, int n)
{ // i este un iterator
  int i;
  // parcurgem fisierul de input
  for (i = 0; i < n; i++)
  { // citim din fisier tipul senzorului
    fscanf(f, "%u", &users[i].user_type);
    // il atribuim in structura senzorului
    fscanf(f, "%s", users[i].nume);
    fscanf(f, "%s", users[i].parola);
    // verificam daca este de tip TIRE sau PMU si facem citirile respective
    if (users[i].user_type == ADMIN)
    { // declaram sensor_data de tipe
      // tire_sensor * si il alocam dinamic

      fscanf(f, "%f", &users[i].key);
      // citim v""ectorul sensor_data al senzorului i
    }
    users[i].cart_total_price = 0;
    users[i].nr_products = 0;
    users[i].cart = malloc(sizeof(product) * INITIAL_SIZE);
  }
}
void rezolvare(user *users, int *n,
               FILE *f)
{ /*folosimt acest while pentru a crea o bucla
     infinita pana la intalnirea comenzii de exit */
  int ok = 1;
  while (ok)
  { /*decaram un char comanda de 13 caractere- reprezentand cea mai
       lunga comanda existenta in fiserele de input*/
    char comanda[50];
    // citim de la tastaturar comanda
    fgets(comanda, 50, stdin);
    /*deoarece comanda fgets pune un \n inainte de \0, punem in loc de\n
     \0*/
    comanda[strlen(comanda) - 1] = '\0';
    /*declaram un index si apelam functia de get_index pentru a stii pe ce
    senzor vom aplica functii*/
    int index;
    index = get_index(comanda);
    /*daca indexul este mai mare decat numarul de senzori sau este negativ
    afisam un mesaj corespunzator*/
    if (index >= *n || index < 0)
      printf("Index not in range!\n");
    else
    { // daca coamnda este de tip print, apelam functia de print
      if (strstr(comanda, "print"))

        // daca coamnda este de tip exit, apelam functia de exit
        if (strstr(comanda, "exit"))
          Exit(users, *n, &ok, f);
      // daca comanda este de tip clear, apelam functia de clear
    }
  }
}
void read_products(FILE *f, product *products, int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    fscanf(f, "%s", products->product_name);
    fscanf(f, "%f", &products->product_price);
    fscanf(f, "%d", &products->product_stock);
  }
}
void show_products(product *products, int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    printf("%d) Numele produsului: %s-->%f LEI-->%d ramase in stoc\n",
           i + 1,
           products[i].product_name, products[i].product_price,
           products[i].product_stock);
  }
}
void print_users(user *users, int n, int current_type)
{
  if (users[current_type].user_type == CUSTOMER)
  {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!");
  }
  else
  {
    int i;
    for (i = 0; i < n; i++)
    {
      printf("Nume utilizator:%s Parola utilizator:%s\n", users[i].nume,
             users[i].parola);
    }
  }
}
void Exit(user *users, int n, int *ok,
          FILE *f)
{ // facem ok-ul 0 pentru a iesi din bucla infinita
  *ok = 0;
  // eliberam memoria utilizata de-a lungul rezolvarii problemei
  freemem(users, n);
  // inchidem fisierul de input
  fclose(f);
}
void add_product(user *users, product *products, int *n, int current_type)
{
  if (users[current_type].user_type == CUSTOMER)
  {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!");
  }
  else
  {
    /*if ((*n) == (sizeof(products) / sizeof(products[0])))
    {
      products = realloc(products, 2 * (*n) * sizeof(products[0]));
    }
    */
    printf("Introduceti numele produsului:\n");
    scanf("%s ", products[(*n)].product_name);
    printf("Introduceti pretul produsului:\n");
    scanf("%f ", &products[(*n)].product_price);
    printf("Introduceti stocul produsului:\n");
    scanf("%d ", &products[(*n)].product_stock);
    (*n)++;
  }
}
void delete_product(user *users, product *products, int *n, int index,
                    int current_type)
{
  if (users[current_type].user_type == CUSTOMER)
  {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!");
  }
  else
  {
    int i;
    for (i = index; i < *n - 1; i++)
    {
      products[i] = products[i + 1];
    }
    (*n)--;
  }
}
void add_to_cart(user *users, int my_index, int nr_produs, product *products)
{
  if ((users[my_index].nr_products) == (sizeof(users[my_index].cart) /
                                        sizeof(product)))
  {
    users[my_index].cart = realloc(users[my_index].cart,
                                   2 * users[my_index].nr_products *
                                       sizeof(product));
  }
  else
  {
    users[my_index].cart[users[my_index].nr_products] = products[nr_produs];
    products[nr_produs].product_stock--;
  }
}
void freemem(user *users, int n)
{ // declaram un iterator
  int i;
  // parcurgem vectorul de senzori
  for (i = 0; i < n; i++)
  {
    // dezalocam memoria alocata dinamic din cadrul fiecarui senzor
    free(users[i].cart);
  }
  // dezalocam memoria propriu-zisa a vectorului de senzori
  free(users);
}
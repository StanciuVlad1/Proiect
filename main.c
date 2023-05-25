#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

// functie care determina index-ul sensorului pe care se aplica o alta functie
int get_index(char s[]);
// functie care citeste sensorii din fisierul de input
void read_users(FILE *f, user *users, int n);
// functie care sorteaza senzorii in functie de importanta (PMU > TIRE)
// functia care citeste produsele din stock-ul magazinului
void read_products(FILE *f, product *products, int n);
// functia de exit necesara problemei
void Exit(user *users, product *products, int n, int *ok, FILE *f, FILE *g);
// functie care sterge un anumit senzor din vectorul de senzori
void add_product(user *users, product *products, int *n, int current_type,
                 int *capacity);
void delete_product(user *users, product *products, int *n, int index,
                    int current_type);
void show_products(product *products, int n);
void add_to_cart(user *users, int my_index, int nr_produs, product *products);
// functia elibereaza memoria alocata dinamic
void login(user *users, int n, int *my_index);
void freemem(user *users, int n, product *products);
void show_users(user *users, int n, int current_type);
void show_cart(user *users, int my_index);
void remove_from_cart(user *users, int my_index, int index);

int main(int argc, char const *argv[])
{ // se deschide fisierul de intrare
  FILE *f = fopen(argv[1], "r");
  if (f == NULL)
  {
    printf("Eroarea la creearea fisierului!\n");
    return 1;
  }

  int nr_users;
  int nr_products;

  fscanf(f, "%d", &nr_users);

  user *users;
  users = (user *)malloc(sizeof(user) * (nr_users));
  read_users(f, users, nr_users);
  FILE *g = fopen(argv[2], "r");
  if (g == NULL)
  {
    printf("Eroarea la creearea fisierului!\n");
    return 1;
  }
  fscanf(g, "%d", &nr_products);
  product *products;
  products = (product *)malloc(sizeof(product) * (nr_products));
  read_products(g, products, nr_products);

  int my_index = -1;
  login(users, nr_users, &my_index);

  int ok = 1;
  while (ok && (my_index) != -1)
  { /*decaram un char comanda de 13 caractere- reprezentand cea mai
       lunga comanda existenta in fiserele de input*/
    char comanda[50];
    // citim de la tastatura comanda
    fgets(comanda, 50, stdin);
    /*deoarece comanda fgets pune un \n inainte de \0, punem in loc de\n
     \0*/
    comanda[strlen(comanda) - 1] = '\0';
    /*declaram un index si apelam functia de get_index pentru a stii pe ce
    senzor vom aplica functii*/

    /*daca indexul este mai mare decat numarul de senzori sau este negativ
    afisam un mesaj corespunzator*/
    // daca coamnda este de tip print, apelam functia de print
    if (strstr(comanda, "add product"))
      add_product(users, products, &nr_products, users[my_index].user_type, &users[my_index].capacity);
    if (strstr(comanda, "delete product"))
    {
      int index = get_index(comanda) - 1;
      delete_product(users, products, &nr_products, index, users[my_index].user_type);
    }
    if (strstr(comanda, "show products"))
      show_products(products, nr_products);
    if (strstr(comanda, "add to cart"))
    {
      int nr_produs = get_index(comanda) - 1;
      add_to_cart(users, my_index, nr_produs, products);
    }
    if (strstr(comanda, "show users"))
      show_users(users, nr_users, users[my_index].user_type);
    if (strstr(comanda, "show cart"))
      show_cart(users, my_index);
    // daca coamnda este de tip exit, apelam functia de exit
    if (strstr(comanda, "remove from cart"))
    {
      int nr_produs = get_index(comanda) - 1;
      remove_from_cart(users, my_index, nr_produs);
    }
    if (strstr(comanda, "exit"))
      Exit(users, products, nr_users, &ok, f, g);
    // daca comanda este de tip clear, apelam functia de clear
  }
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
    // verificam daca este de tip ADMIN sau CUSTOMER si facem citirile
    // respective
    users[i].cart_total_price = 0;
    users[i].nr_products = 0;
    users[i].capacity = 100;
    users[i].cart = malloc(sizeof(product) * users[i].capacity);
  }
}

void read_products(FILE *f, product *products, int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    fscanf(f, "%s", products[i].product_name);
    fscanf(f, "%f", &products[i].product_price);
    fscanf(f, "%d", &products[i].product_stock);
  }
}
void show_products(product *products, int n)
{
  int i;
  for (i = 0; i < n; i++)
  {
    printf("%d) Numele produsului: %s-->%0.2f LEI-->%d ramase in stoc\n", i + 1,
           products[i].product_name, products[i].product_price,
           products[i].product_stock);
  }
}
void show_users(user *users, int n, int current_type)
{
  if (users[current_type].user_type == CUSTOMER)
  {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!\n");
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
void Exit(user *users, product *products, int n, int *ok,
          FILE *f, FILE *g)
{ // facem ok-ul 0 pentru a iesi din bucla infinita
  *ok = 0;
  // eliberam memoria utilizata de-a lungul rezolvarii problemei
  freemem(users, n, products);
  // inchidem fisierul de input
  fclose(f);
  fclose(g);
}
void add_product(user *users, product *products, int *n, int current_type,
                 int *capacity)
{
  if (users[current_type].user_type == CUSTOMER)
  {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!\n");
  }
  else
  {
    if ((*n) == (*capacity))
    {
      (*capacity) *= 2;
      products = realloc(products, (*capacity) * sizeof(products[0]));
    }
    printf("Introduceti numele produsului:\n");
    fgets(products[(*n)].product_name, 30, stdin);
    products[(*n)].product_name[strlen(products[(*n)].product_name) - 1] = '\0';
    printf("Introduceti pretul produsului:\n");
    scanf("%f", &products[(*n)].product_price);
    printf("Introduceti stocul produsului:\n");
    scanf("%d", &products[(*n)].product_stock);
    (*n)++;
  }
}
void delete_product(user *users, product *products, int *n, int index,
                    int current_type)
{
  if (users[current_type].user_type == CUSTOMER)
  {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!\n");
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
  if (products[nr_produs].product_stock == 0)
  {
    printf("Produsul nu mai este in stoc\n");
  }
  else
  {
    if ((users[my_index].nr_products) == users[my_index].capacity)
    {
      users[my_index].capacity *= 2;
      users[my_index].cart = realloc(
          users[my_index].cart, users[my_index].capacity * sizeof(product));
    }
    users[my_index].cart[users[my_index].nr_products] = products[nr_produs];
    products[nr_produs].product_stock--;
    users[my_index].nr_products++;
    users[my_index].cart_total_price += products[nr_produs].product_price;
  }
}
void freemem(user *users, int n, product *products)
{ // declaram un iterator

  // parcurgem vectorul de senzori
  int i;
  for (i = 0; i < n; i++)
    free(users[i].cart);
  // dezalocam memoria propriu-zisa a vectorului de senzori
  free(users);
  free(products);
}
void login(user *users, int n, int *my_index)
{
  int auxiliar = -1;
  printf("Introduceti numele de utilizator : ");
  char nume[50];
  fgets(nume, 50, stdin);
  nume[strlen(nume) - 1] = '\0';
  int i;
  for (i = 0; i < n; i++)
  {
    if (!strcmp(users[i].nume, nume))
    {
      printf("\nIntroduceti parola : ");
      char password[50];
      fgets(password, 50, stdin);
      password[strlen(password) - 1] = '\0';
      if (!strcmp(users[i].parola, password))
      {
        auxiliar = i;
        printf("V-ati autentificat cu succes!\n");
        break;
      }
      else
      {
        auxiliar = -1;
      }
    }
    else
    {
      auxiliar = -1;
    }
  }
  *my_index = auxiliar;
}
void show_cart(user *users, int my_index)
{
  int i;
  for (i = 0; i < users[my_index].nr_products; i++)
    printf("%d) Numele produsului: %s-->%0.2f LEI\n", i + 1,
           users[my_index].cart[i].product_name, users[my_index].cart[i].product_price);
  printf("Costul total al cosului de cumparaturi este : %0.2f\n", users[my_index].cart_total_price);
}
void remove_from_cart(user *users, int my_index, int index)
{
  if (users[my_index].nr_products == 0)
  {
    printf("Cosul tau este deja gol!\n");
  }
  else
  {
    users[my_index].cart_total_price -= users[my_index].cart[index].product_price;
    int i;
    for (i = index; i < users[my_index].nr_products - 1; i++)
    {
      users[my_index].cart[i] = users[my_index].cart[i + 1];
    }
    users[my_index].nr_products--;
  }
}
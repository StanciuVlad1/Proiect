#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"

// functie care determina index-ul prosudului pe care se aplica o alta functie
int get_index(char s[]);
// functie care citeste utilizatorii din fisierul de input
void read_users(FILE *f, user *users, int n);
// functia care citeste produsele din stocul magazinului
void read_products(FILE *f, product *products, int n);
// functia de exit necesara problemei
void Exit(user *users, product *products, int n, int *ok, FILE *f, FILE *g);
// functie care autentifica utilizatorul
void login(user *users, int n, int *my_index);
// functie care afiseaza toti utilizatorii
void show_users(user *users, int n, int current_type);
// functie care adauga un produs
void add_product(user *users, product *products, int *n, int current_type,
                 int *capacity);
// functie care sterge un produs
void delete_product(user *users, product *products, int *n, int index,
                    int current_type);
// functie care afiseaza toate produsele
void show_products(product *products, int n);
// functie care adauga un produs in cosul de cumparaturi
void add_to_cart(user *users, int my_index, int nr_produs, product *products);
// functie care sterge un produs din cosul de cumparaturi
void remove_from_cart(user *users, int my_index, int index);
// functie care afiseaza produsele din cosul de cumparaturi
void show_cart(user *users, int my_index);
// functie de eliberare a intregii memorii alocate pe parcursul programului
void freemem(user *users, int n, product *products);

int main(int argc, char const *argv[]) {
  // se deschide primul fisier de intrare si verificam daca s-a putut deschide
  FILE *f = fopen(argv[1], "r");
  if (f == NULL) {
    printf("Eroarea la creearea fisierului!\n");
    return 1;
  }
  int nr_users;
  int nr_products;
  // citim numarul de utilizatori
  fscanf(f, "%d", &nr_users);
  // alocam memorie pentru utilizatori si verificam daca s-a putut face alocarea
  user *users;
  users = (user *)malloc(sizeof(user) * (nr_users));
  if (users == NULL) {
    printf("Alocarea userilor nu a fost posibila\n");
    return 0;
  }
  // citim din fisier utilizatorii
  read_users(f, users, nr_users);
  // se deschide al doilea fisier de intrare si verificam daca s-a putut
  // deschide
  FILE *g = fopen(argv[2], "r");
  if (g == NULL) {
    printf("Eroarea la creearea fisierului!\n");
    return 1;
  }
  // citim numarul de produse
  fscanf(g, "%d", &nr_products);
  // alocam memorie pentru produse si verificam daca s-a putut face alocarea
  product *products;
  products = (product *)malloc(sizeof(product) * (nr_products));
  if (products == NULL) {
    printf("Alocarea produselor nu a fost posibila\n");
    return 0;
  }
  // citim din fisier produsele
  read_products(g, products, nr_products);
  // declaram indexul cu o valoare invalida
  int my_index = -1;
  // apelam functia de autentificare si determinam indexul utilizatorului
  login(users, nr_users, &my_index);
  // efectuam comenzile pana la comanda exit sau nu avem un index invalid
  int ok = 1;
  // decaram un char comanda de 50 caractere
  char comanda[50];
  while (ok && (my_index) != -1) {
    // citim de la tastatura comanda
    fgets(comanda, 50, stdin);
    /*deoarece comanda fgets pune un \n inainte de \0, punem in loc de\n
     \0*/
    comanda[strlen(comanda) - 1] = '\0';
    // daca coamanda este de tip add product, apelam functia add_product
    if (strstr(comanda, "add product"))
      add_product(users, products, &nr_products, users[my_index].user_type,
                  &users[my_index].capacity);
    // daca coamanda este de tip delete product, apelam functia delete_product
    if (strstr(comanda, "delete product")) {
      // declaram un index si apelam functia de get_index pentru a stii pe ce
      // produs vom aplica functia
      int index = get_index(comanda) - 1;
      delete_product(users, products, &nr_products, index,
                     users[my_index].user_type);
    }
    // daca coamanda este de tip show products, apelam functia show_products
    if (strstr(comanda, "show products")) {
      show_products(products, nr_products);
    }
    // daca coamanda este de tip add to cart, apelam functia add_to_cart
    if (strstr(comanda, "add to cart")) {
      // declaram un index si apelam functia de get_index pentru a stii pe ce
      // produs vom aplica functia
      int nr_produs = get_index(comanda) - 1;
      add_to_cart(users, my_index, nr_produs, products);
    }
    // daca coamanda este de tip show users, apelam functia show_users
    if (strstr(comanda, "show users")) {
      show_users(users, nr_users, users[my_index].user_type);
    }
    // daca coamanda este de tip show cart, apelam functia show_cart
    if (strstr(comanda, "show cart")) {
      show_cart(users, my_index);
    }
    // daca coamanda este de tip remove from cart, apelam functia
    // remove_from_cart
    if (strstr(comanda, "remove from cart")) {
      int nr_produs = get_index(comanda) - 1;
      remove_from_cart(users, my_index, nr_produs);
    }
    // daca coamnda este de tip exit, apelam functia de exit
    if (strstr(comanda, "exit")) Exit(users, products, nr_users, &ok, f, g);
  }
  return 0;
}

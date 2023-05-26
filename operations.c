#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#define PRODUCT_NAME_SIZE 30
#define USER_NAME_SIZE 50
#define USER_PASSWORD_SIZE 20
// ------- DO NOT MODIFY ------- //

int get_index(char s[]) {
  // declaram nr, care reprezinta numarul nostru ce se afla in comanda
  // i este un iterator care ne va ajuta sa ne plimbam prin vectorul de tip char
  // negativ ne va spune daca in "s" se va intalni semnul "-"
  int nr = 0, i, negativ = 0;
  // parcurgem comanda data de la tastatura
  for (i = 0; i < strlen(s); i++) {
    // verificam daca caracterul de pe pozitia curenta este cifra
    if (s[i] >= '0' && s[i] <= '9')
      // in caz afirmativ este adaugata la numarul nostru
      nr = nr * 10 + (s[i] - '0');
    // daca intalnim semnul "-", negativ va deveni 1
    if (s[i] == '-') negativ = 1;
  }
  // in cazul in care numarul nostru este negativ, il inmultim cu (-1)
  if (negativ) nr = -nr;
  // returnam numarul obtinut
  return nr;
}

void read_users(FILE *f, user *users, int n) {
  int i;
  // parcurgem fisierul de input
  for (i = 0; i < n; i++) {
    // citim din fisier tipul,numele si parola utilizatorului
    fscanf(f, "%u", &users[i].user_type);
    fscanf(f, "%s", users[i].nume);
    fscanf(f, "%s", users[i].parola);
    // initializam campurile
    users[i].cart_total_price = 0;
    users[i].nr_products = 0;
    users[i].capacity = 100;
    // alocam meorie pentru cosul de cumparaturi si verificam daca s-a putut
    // realiza
    users[i].cart = malloc(sizeof(product) * users[i].capacity);
    if (users[i].cart == NULL) {
      printf("Alocarea cosului de cumparaturi nu a fost posibila\n");
      exit(-1);
    }
  }
}

void read_products(FILE *f, product *products, int n) {
  int i;
  // parcurgem fisierul de input
  for (i = 0; i < n; i++) {
    // citim din fisier numele, pretul si stocul utilizatorului
    fscanf(f, "%s", products[i].product_name);
    fscanf(f, "%f", &products[i].product_price);
    fscanf(f, "%d", &products[i].product_stock);
  }
}

void show_products(product *products, int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("%d) Numele produsului: %s-->%0.2f LEI-->%d ramase in stoc\n", i + 1,
           products[i].product_name, products[i].product_price,
           products[i].product_stock);
  }
}

// in cadrul functiei acesteia, doar adminii au permisiunea de a afisa toti
// utilizatorii
void show_users(user *users, int n, int current_type) {
  if (current_type == CUSTOMER) {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!\n");
  } else {
    int i;
    for (i = 0; i < n; i++) {
      printf("Nume utilizator:%s Parola utilizator:%s\n", users[i].nume,
             users[i].parola);
    }
  }
}
void freemem(user *users, int n, product *products) {
  int i;
  for (i = 0; i < n; i++) free(users[i].cart);
  free(users);
  free(products);
}

// functie de iesire din bucla de comenzi
void Exit(user *users, product *products, int n, int *ok, FILE *f, FILE *g) {
  // facem ok-ul 0 pentru a iesi din bucla infinita
  *ok = 0;
  // eliberam memoria utilizata de-a lungul rezolvarii problemei
  freemem(users, n, products);
  // inchidem fisierele de input
  fclose(f);
  fclose(g);
}

// in cadrul acestei functii, doar adminii au permisiunea de a adauga un produs
// in stoc
void add_product(user *users, product *products, int *n, int current_type,
                 int *capacity) {
  if (current_type == CUSTOMER) {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!\n");
  } else {
    // daca am depasit capacitatea, realocam prin dublarea capacitatii
    if ((*n) == (*capacity)) {
      (*capacity) *= 2;
      products = realloc(products, (*capacity) * sizeof(products[0]));
      if (products == NULL) {
        printf("Realocarea produselor nu a fost posibila\n");
        exit(-1);
      }
    }
    printf("Introduceti numele produsului:\n");
    // citim produsul nou
    fgets(products[(*n)].product_name, PRODUCT_NAME_SIZE, stdin);
    products[(*n)].product_name[strlen(products[(*n)].product_name) - 1] = '\0';
    printf("Introduceti pretul produsului:\n");
    scanf("%f", &products[(*n)].product_price);
    printf("Introduceti stocul produsului:\n");
    scanf("%d", &products[(*n)].product_stock);
    (*n)++;
  }
}

// in cadrul acestei functii, doar adminii au permisiunea de a sterge un produs
// din stoc
void delete_product(user *users, product *products, int *n, int index,
                    int current_type) {
  if (users[current_type].user_type == CUSTOMER) {
    printf("NU AVETI PERMISIUNEA DE A UTILIZA ACEASTA COMANDA!\n");
  } else {
    int i;
    for (i = index; i < *n - 1; i++) {
      products[i] = products[i + 1];
    }
    (*n)--;
  }
}

void add_to_cart(user *users, int my_index, int nr_produs, product *products) {
  // adaugam in cosul de cumparaturi doar daca mai este in stoc produsul
  // respectiv
  if (products[nr_produs].product_stock == 0) {
    printf("Produsul nu mai este in stoc\n");
  } else {
    // daca am depasit capacitatea, realocam prin dublarea capacitatii
    if ((users[my_index].nr_products) == users[my_index].capacity) {
      users[my_index].capacity *= 2;
      users[my_index].cart = realloc(
          users[my_index].cart, users[my_index].capacity * sizeof(product));
      if (users[my_index].cart == NULL) {
        printf("Realocarea cosului de cumparaturi nu a fost posibila\n");
        exit(-1);
      }
    }
    // adaug produsul
    users[my_index].cart[users[my_index].nr_products] = products[nr_produs];
    // scotem produsul din stoc deoarece a fost adaugat in cosul de cumparaturi
    products[nr_produs].product_stock--;
    // marim numarul de produse din cosul de cumparaturi
    users[my_index].nr_products++;
    // recalculam pretul total al cosului de cumparaturi
    users[my_index].cart_total_price += products[nr_produs].product_price;
  }
}

void login(user *users, int n, int *my_index) {
  int auxiliar = -1;
  printf("Introduceti numele de utilizator : ");
  char nume[USER_NAME_SIZE];
  fgets(nume, USER_NAME_SIZE, stdin);
  nume[strlen(nume) - 1] = '\0';
  int i;
  for (i = 0; i < n; i++) {
    // daca numele se potriveste, continuam cu introducerea parolei
    if (!strcmp(users[i].nume, nume)) {
      printf("\nIntroduceti parola : ");
      char password[USER_PASSWORD_SIZE];
      fgets(password, USER_PASSWORD_SIZE, stdin);
      password[strlen(password) - 1] = '\0';
      // daca si parola se potriveste, autentificarea a avut succes
      if (!strcmp(users[i].parola, password)) {
        auxiliar = i;
        printf("V-ati autentificat cu succes!\n");
        break;
      } else {
        auxiliar = -1;
      }
    } else {
      auxiliar = -1;
    }
  }
  // daca numele sau parola au fost gresite, indexul va fi invalid si nu se va
  // efectua autentificarea
  *my_index = auxiliar;
}

void show_cart(user *users, int my_index) {
  int i;
  for (i = 0; i < users[my_index].nr_products; i++)
    printf("%d) Numele produsului: %s-->%0.2f LEI\n", i + 1,
           users[my_index].cart[i].product_name,
           users[my_index].cart[i].product_price);
  printf("Costul total al cosului de cumparaturi este : %0.2f\n",
         users[my_index].cart_total_price);
}

void remove_from_cart(user *users, int my_index, int index) {
  // daca produsul nu mai exista pe stoc, nu se poate realiza eliminarea
  // acestuia din stoc
  if (users[my_index].nr_products == 0) {
    printf("Cosul tau este deja gol!\n");
  } else {
    // recalculam pretul total al cosului de cumparaturi
    users[my_index].cart_total_price -=
        users[my_index].cart[index].product_price;
    int i;
    for (i = index; i < users[my_index].nr_products - 1; i++) {
      users[my_index].cart[i] = users[my_index].cart[i + 1];
    }
    users[my_index].nr_products--;
  }
}

// ------- DO NOT MODIFY ------- //

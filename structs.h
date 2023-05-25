#ifndef _STRUCTS_H_
#define _STRUCTS_H_

enum user_type { ADMIN, CUSTOMER };

typedef struct {
  char product_name[30];
  float product_price;
  int product_stock;
} product;

typedef struct {
  enum user_type user_type;
  char nume[50];
  char parola[20];
  float key;
  float cart_total_price;
  product *cart;
  int nr_products;
  int capacity;
} user;

#endif  // _STRUCTS_H_
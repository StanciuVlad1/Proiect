#ifndef _STRUCTS_H_
#define _STRUCTS_H_

enum user_type { ADMIN, CUSTOMER };

typedef struct {
  enum user_type user_type;
  void *user_data;
  char nume[50];
  char parola[20];
  float key;
  float cart_total_price;
  product *cart;
  int nr_products;
} user;

typedef struct {
  char product_name[30];
  float product_price;
  int product_stock;
} product;

#endif  // _STRUCTS_H_
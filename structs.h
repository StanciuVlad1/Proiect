#ifndef _STRUCTS_H_
#define _STRUCTS_H_
#define PRODUCT_NAME_SIZE 30
#define USER_NAME_SIZE 50
#define USER_PASSWORD_SIZE 20

enum user_type { ADMIN, CUSTOMER };

typedef struct {
  char product_name[PRODUCT_NAME_SIZE];
  float product_price;
  int product_stock;
} product;

typedef struct {
  enum user_type user_type;
  char nume[USER_NAME_SIZE];
  char parola[USER_PASSWORD_SIZE];
  float cart_total_price;
  product *cart;
  int nr_products;
  int capacity;
} user;

#endif  // _STRUCTS_H_
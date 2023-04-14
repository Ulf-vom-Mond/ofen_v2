#pragma once

#include <stdint.h>

struct list{
  void *data;
  struct list *next;
};

struct list *new_list();
void add_last(struct list *list, void *data);
void add_next(struct list *list, void *data);
struct list *get_next(struct list *list);
void *get_data_ptr(struct list *list);
void remove_next(struct list *list);
uint16_t list_size(struct list *list);
void remove_last(struct list *list);
void delete_list(struct list *list);


int freeMemory();
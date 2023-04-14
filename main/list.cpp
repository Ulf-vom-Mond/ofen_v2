#include "list.h"


struct list *new_list() {
  struct list *new_list = new struct list;
  new_list->data = 0;
  new_list->next = 0;
  return new_list;
}

void add_last(struct list *list, void *data) {
  struct list *last_element = list;
  while(last_element->next) {
    last_element = last_element->next;
  }
  last_element->data = data;
  last_element->next = new_list();
}

void add_next(struct list *list, void *data) {
  struct list *new_element = new_list();
  new_element->data = data;
  new_element->next = list->next;
  list->next = new_element;
}

struct list *get_next(struct list *list) {
  return list->next;
}

void *get_data_ptr(struct list *list) {
  return list->data;
}

void remove_next(struct list *list) {
  if(!list->next) {return;}
  struct list *new_next = list->next->next;
  delete list->next;
  list->next = new_next;
}

uint16_t list_size(struct list *list) {
  uint16_t size = 0;
  struct list *last_element = list;
  while(last_element->next) {
    last_element = last_element->next;
    size++;
  }
  return size;
}

void remove_last(struct list *list) {
  uint16_t size = list_size(list);

  if (size == 0) {
    delete list;
    return;
  }

  struct list *second_to_last = list;
  for (int i = 0; i < size - 1; i++) {
    second_to_last = second_to_last->next;
  }

  remove_next(second_to_last);
}

void delete_list(struct list *list) { // doesnt work, this function wont free any memory
  if(list->next->next) {
    delete_list(list->next);
  }
  remove_next(list);
  return;
}
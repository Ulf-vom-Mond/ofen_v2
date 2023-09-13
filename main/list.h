#pragma once

#include <stdint.h>

template <typename T>
struct list{
  T data;
  struct list *next;
};

template <typename T>
struct list<T> *new_list();
template <typename T>
void add_last(struct list<T> *list, T data);
template <typename T>
void add_next(struct list<T> *list, T data);
template <typename T>
struct list<T> *get_next(struct list<T> *list);
template<typename T>
struct list<T> *get_index(struct list<T> *list, uint16_t index);
template <typename T>
T get_data_ptr(struct list<T> *list);
template <typename T>
void remove_next(struct list<T> *list);
template <typename T>
list<T>* remove_first(struct list<T> *list);
template <typename T>
uint16_t list_size(struct list<T> *list);
template <typename T>
void remove_last(struct list<T> *list);
template <typename T>
void delete_list(struct list<T> *list);


template <typename T>
struct list<T> *new_list() {
  struct list<T> *new_list = new struct list<T>;
  //new_list->data = (T)0;
  new_list->next = 0;
  return new_list;
}

template <typename T>
void add_last(struct list<T> *list, T data) {
  struct list<T> *last_element = list;
  while(last_element->next) {
    last_element = last_element->next;
  }
  last_element->data = data;
  last_element->next = new_list<T>();
}

template <typename T>
void add_next(struct list<T> *list, T data) {
  struct list<T> *new_element = new_list<T>();
  new_element->data = data;
  new_element->next = list->next;
  list->next = new_element;
}

template <typename T>
struct list<T> *get_next(struct list<T> *list) {
  return list->next;
}

template<typename T>
struct list<T> *get_index(struct list<T> *list, uint16_t index) {
  struct list<T> *list_iter = list;
  for(uint16_t i = 0; list_iter->next && i < index; i++) {
    list_iter = list_iter->next;
  }
  return list_iter;
}

template <typename T>
T get_data_ptr(struct list<T> *list) {
  return list->data;
}

template <typename T>
void remove_next(struct list<T> *list) {
  if(!list->next) {return;}
  struct list<T> *new_next = list->next->next;
  delete list->next;
  list->next = new_next;
}

template <typename T>
list<T>* remove_first(struct list<T> *list) {
  return list->next;
}

template <typename T>
uint16_t list_size(struct list<T> *list) {
  uint16_t size = 0;
  struct list<T> *last_element = list;
  while(last_element->next) {
    last_element = last_element->next;
    size++;
  }
  return size;
}

template <typename T>
void remove_last(struct list<T> *list) {
  uint16_t size = list_size<T>(list);

  if (size == 0) {
    delete list;
    return;
  }

  struct list<T> *second_to_last = list;
  for (int i = 0; i < size - 1; i++) {
    second_to_last = second_to_last->next;
  }

  remove_next(second_to_last);
}

template <typename T>
void delete_list(struct list<T> *list) { // doesnt work, this function wont free any memory
  if(list->next->next) {
    delete_list<T>(list->next);
  }
  remove_next<T>(list);
  return;
}
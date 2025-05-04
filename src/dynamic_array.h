#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *data;
  size_t size;
  size_t capacity;
} dynamic_array;

dynamic_array *create_array(size_t initial_capacity);
void push(dynamic_array *array, int value);
int pop(dynamic_array *array);
int get(dynamic_array *array, size_t index);
void set(dynamic_array *array, size_t index, int value);
void free_dynamic_array(dynamic_array *array);
size_t size(dynamic_array *array);
size_t capacity(dynamic_array *array);
int resize(dynamic_array *array, size_t new_capacity);
void clear_dynamic_array(dynamic_array *array);
void print_dynamic_array(dynamic_array *array, size_t begin, size_t end);
int begin_dynamic_array(dynamic_array *array);
int end_dynamic_array(dynamic_array *array);
int pop_at(dynamic_array *array, size_t index);

#endif

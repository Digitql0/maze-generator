#include "dynamic_array.h"
#include <stdio.h>
#include <stdlib.h>

// creating a pointer to the Array
dynamic_array *create_array(size_t initial_capacity) {

  // Allocating the Memory for the Struct to save its Variables.
  dynamic_array *array = (dynamic_array *)malloc(sizeof(dynamic_array));
  if (array == NULL) {
    fprintf(stderr, "Memory Allocation of Array Struct Failed.\n");
    return NULL;
  }

  // Allocating some Memory for the Data about to be stored.
  array->data = (int *)malloc(sizeof(int) * initial_capacity);
  if (array->data == NULL) {
    free(array);
    fprintf(stderr, "Memory Allocation of Array Data.\n");
    return NULL;
  }

  array->capacity = initial_capacity;
  array->size = 0;

  return array;
}

void push(dynamic_array *array, int value) {
  if (array->size >= array->capacity) {

    size_t new_capacity = array->capacity * 2;
    int *new_data = (int *)realloc(array->data, sizeof(int) * new_capacity);

    if (new_data == NULL) {
      fprintf(stderr, "Memory Allocation for new Memory failed.\n");
      return;
    }

    array->data = new_data;
    array->capacity = new_capacity;
  }

  array->data[array->size] = value;
  array->size++;
}

int pop(dynamic_array *array) {

  if (array->size <= 0) {

    fprintf(stderr, "Array is already empty.\n");
    return -1;
  }

  int popped_value = array->data[array->size - 1];
  array->size--;

  if (array->size > 0 && array->size <= array->capacity / 2) {

    size_t new_capacity = array->capacity / 2;
    int *new_data = (int *)realloc(array->data, sizeof(int) * new_capacity);

    if (new_data != NULL) {

      array->data = new_data;
      array->capacity = new_capacity;

    } else {

      fprintf(stderr, "Memory Allocation of new data has failed.\n");
    }
  }

  return popped_value;
}

int get(dynamic_array *array, size_t index) {
  if (index < array->size) {

    return array->data[index];
  }
  fprintf(stderr, "Index is outside array size.\n");
  return -1;
}

void set(dynamic_array *array, size_t index, int value) {
  if (index < array->size) {
    array->data[index] = value;
  } else {
    fprintf(stderr, "Index is outside array size.\n");
  }
}

void free_dynamic_array(dynamic_array *array) {
  if (array != NULL) {
    if (array->data != NULL) {

      free(array->data);

    } else {

      fprintf(stderr, "No Data is Allocated in Memory.\n");
    }

    free(array);

  } else {

    fprintf(stderr, "Array Struct is not alloated in Memory.\n");
  }
}

size_t size(dynamic_array *array) { return array->size; }

size_t capacity(dynamic_array *array) { return array->capacity; }

int resize(dynamic_array *array, size_t new_capacity) {

  if (new_capacity == 0) {

    if (array->data != NULL) {
      free(array->data);
      array->data = NULL;
    }

    array->capacity = 0;
    array->size = 0;
    return 0;
  }

  if (array->size > new_capacity) {

    fprintf(stderr, "Array has too many elements to shrink so much.\n");
    return -1;
  }

  int *new_data = (int *)realloc(array->data, sizeof(int) * new_capacity);

  if (new_data == NULL) {

    fprintf(stderr, "Memory Allocation for new Size has Failed.\n");
    return -1;
  }

  array->data = new_data;
  array->capacity = new_capacity;

  return 0;
}

void clear_dynamic_array(dynamic_array *array) {
  if (array->data != NULL) {
    free(array->data);
    array->data = NULL;
  }
}

void print_dynamic_array(dynamic_array *array, size_t begin, size_t end) {
  if (array == NULL || array->data == NULL) {
    fprintf(stderr, "Array is not initialized properly.\n");
    return;
  }

  if (begin >= array->size || end <= begin || end > array->size) {
    fprintf(stderr, "Invalid range.\n");
    return;
  }

  for (size_t i = begin; i < end; ++i) {
    printf("%d, ", array->data[i]); // Print Integer Values after one another
  }
  printf("\n");
}

int begin_dynamic_array(dynamic_array *array) {
  return (array != NULL && array->size > 0) ? 0 : -1;
}

int end_dynamic_array(dynamic_array *array) {
  return (array != NULL && array->size > 0) ? (array->size - 1) : -1;
}

int pop_at(dynamic_array *array, size_t index) {
  if (index < 0 || index > array->size) {
    fprintf(stderr, "Invalid Index. \n");
    return -1;
  }

  int popped_value = array->data[index];

  for (size_t i = index + 1; i < array->size; ++i) {
    array->data[i - 1] = array->data[i];
  }
  array->size--;

  return popped_value;
}

#include "stack.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void push_c(Stack_c *stack, const C value) {
  if (stack->size >= STACK_MAX_SIZE) {
    exit(STACK_OVERFLOW);
  }
  stack->data[stack->size] = value;
  stack->size++;
}
void push_d(Stack_d *stack_d, const D value) {
  if (stack_d->size_d >= STACK_MAX_SIZE) {
    exit(STACK_OVERFLOW);
  }
  stack_d->data[stack_d->size_d] = value;
  stack_d->size_d++;
}
C pop_c(Stack_c *stack) {
  if (stack->size == 0) {
    exit(STACK_UNDERFLOW);
  }
  stack->size--;
  return stack->data[stack->size];
}
D pop_d(Stack_d *stack_d) {
  if (stack_d->size_d == 0) {
    exit(STACK_UNDERFLOW);
  }
  stack_d->size_d--;
  return stack_d->data[stack_d->size_d];
}
C peek_c(const Stack_c *stack) {
  if (stack->size <= 0) {
    exit(STACK_UNDERFLOW);
  }
  return stack->data[stack->size - 1];
}
// D peek_d(const Stack_d *stack_d) {
//   if (stack_d->size_d <= 0) {
//     exit(STACK_UNDERFLOW);
//   }
//   return stack_d->data[stack_d->size_d - 1];
// }
// void printStackValue_C(const C value) { printf("%d", value); }
// void printStack_c(const Stack_c *stack, void (*printStackValue_c)(const C)) {
//   int i;
//   int len = stack->size - 1;
//   printf("stack %d > ", stack->size);
//   for (i = 0; i < len; i++) {
//     printStackValue_c(stack->data[i]);
//     printf(" | ");
//   }
//   if (stack->size != 0) {
//     printStackValue_c(stack->data[i]);
//   }
//   printf("\n");
// }
// void printStackValue_d(const double value) { printf("%f", value); }
// void printStack_d(const Stack_d *stack_d,
//                   void (*printStackValue_d)(const double)) {
//   int i;
//   int len = stack_d->size_d - 1;
//   printf("stack %d > ", stack_d->size_d);
//   for (i = 0; i < len; i++) {
//     printStackValue_d(stack_d->data[i]);
//     printf(" | ");
//   }
//   if (stack_d->size_d != 0) {
//     printStackValue_d(stack_d->data[i]);
//   }
//   printf("\n");
// }
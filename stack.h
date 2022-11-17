#ifndef SRC_STACK_H_
#define SRC_STACK_H_

#define STACK_OVERFLOW -100
#define STACK_UNDERFLOW -101
#define STACK_MAX_SIZE 1024

typedef char C;
typedef double D;

typedef struct Stack_char {
  C data[STACK_MAX_SIZE];
  unsigned int size;
} Stack_c;

typedef struct Stack_dobule {
  double data[STACK_MAX_SIZE];
  unsigned int size_d;
} Stack_d;

void push_c(Stack_c *stack, const C value);
void push_d(Stack_d *stack_d, const D value);
C pop_c(Stack_c *stack);
D pop_d(Stack_d *stack_d);
C peek_c(const Stack_c *stack);
// D peek_d(const Stack_d *stack_d);

// void printStackValue_C(const C value);
// void printStack_c(const Stack_c *stack, void (*printStackValue_c)(const C));

// void printStackValue_d(const D value);
// void printStack_d(const Stack_d *stack_d, void (*printStackValue_d)(const
// D));

#endif  // SRC_STACK_H_

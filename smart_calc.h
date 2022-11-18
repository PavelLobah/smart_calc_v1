#ifndef SRC_SMART_CALC_H_
#define SRC_SMART_CALC_H_
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

char *to_postfix(char *infix_expr, char *postfix_expr);
double calc(const char *str, Stack_d stack_d, double X);

void change_unari_plus_minus(char *str);
int operation_priority(char token);
void tokenize_operator(char *str);
double execute(char op, double first, double second);
double execute_unari(char op, double value);

int end_of_the_word(char *str, size_t i);
double stod(const char *s);

void delete_null(char *str);
int check(char *str);

#endif  // SMART_CALC_H_
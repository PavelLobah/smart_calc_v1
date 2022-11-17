#include "mart_calc.h"

double calc(const char *str, Stack_d stack_d, double X) {
  double res = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    char c = str[i];
    double val;
    int j = 0;
    char numbers[100] = {0};

    if ((c >= '0' && c <= '9') || c == '.' || c == ',' || c == 'X') {
      if (c == 'X') {
        push_d(&stack_d, X);
      } else {
        while (!(str[i] == ' ')) {
          numbers[j] = str[i];
          i++;
          j++;
        }
        val = stod(numbers);
        push_d(&stack_d, val);
      }

    } else if (c == '%' || c == '+' || c == '-' || c == '*' || c == '~' ||
               c == '/' || c == '^') {
      if (c == '~') {
        val = pop_d(&stack_d) * (-1);
        push_d(&stack_d, val);
      } else {
        double second = pop_d(&stack_d);
        double first = pop_d(&stack_d);
        double result = execute(c, first, second);
        push_d(&stack_d, result);
      }
    } else if (c == 'c' || c == 's' || c == 't' || c == 'C' || c == 'S' ||
               c == 'T' || c == 'q' || c == 'l' || c == 'L') {
      double value = pop_d(&stack_d);
      double result = execute_unari(c, value);
      push_d(&stack_d, result);
    }
  }

  return res = pop_d(&stack_d);
}

char *to_postfix(char *infix_expr_orig, char *postfix_expr) {
  Stack_c stack_c;
  stack_c.size = 0;
  int j = 0;
  char infix_expr[256] = {0};
  strcpy(infix_expr, infix_expr_orig);
  tokenize_operator(infix_expr);
  change_unari_plus_minus(infix_expr);

  for (size_t i = 0; i < strlen(infix_expr); i++) {
    char c = infix_expr[i];
    if ((c >= '0' && c <= '9') || c == '.' || c == 'X') {
      if (!((infix_expr[i + 1] >= '0' && infix_expr[i + 1] <= '9') ||
            infix_expr[i + 1] == '.')) {
        postfix_expr[j++] = infix_expr[i];
        postfix_expr[j++] = ' ';
      } else {
        postfix_expr[j++] = infix_expr[i];
      }

    } else if (c == '(') {
      push_c(&stack_c, c);
    } else if (c == ')') {
      while (peek_c(&stack_c) != '(') {
        postfix_expr[j++] = pop_c(&stack_c);
        postfix_expr[j++] = ' ';
      }
      pop_c(&stack_c);
    } else if (c == '%' || c == '+' || c == '-' || c == '*' || c == '~' ||
               c == '/' || c == '^' || c == 'c' || c == 's' || c == 't' ||
               c == 'C' || c == 'S' || c == 'T' || c == 'q' || c == 'l' ||
               c == 'L') {
      char op = c;
      while (stack_c.size > 0 &&
             (operation_priority(peek_c(&stack_c)) >= operation_priority(op))) {
        postfix_expr[j++] = pop_c(&stack_c);
        postfix_expr[j++] = ' ';
      }
      push_c(&stack_c, op);
    }
  }
  for (; stack_c.size > 0;) {
    postfix_expr[j] = pop_c(&stack_c);
    j++;
  }
  postfix_expr[j] = '\0';

  return postfix_expr;
}

int operation_priority(char token) {
  int result = 0;
  // if (token == '#' || token == '~') {
  //     result = 5;
  // } else

  if (token == 'c' || token == 's' || token == 't' || token == 'C' ||
      token == 'S' || token == 'T' || token == 'q' || token == 'l' ||
      token == 'L') {
    result = 4;
  } else if (token == '^' || token == '#' || token == '~') {
    result = 3;
  } else if (token == '*' || token == '/' || token == '%') {
    result = 2;
  } else if (token == '+' || token == '-') {
    result = 1;
  } else if (token == '(' || token == ')') {
    result = 0;
  }
  return result;
}

void change_unari_plus_minus(char *str) {
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[0] == '-') {
      str[0] = '~';
    }
    if (str[i] == '-' && str[i - 1] == '(') {
      str[i] = '~';
    }
    if (str[0] == '+') {
      str[0] = '#';
    }
    if (str[i] == '+' && str[i - 1] == '(') {
      str[i] = '#';
    }
  }
}

void tokenize_operator(char *str) {
  char result[strlen(str) + 1];
  int j = 0;
  for (size_t i = 0; i < strlen(str); i++) {
    if (str[i] == 'A') {
      i++;
      if (str[i] == 'C') {
        result[j++] = 'C';  // acos
        i = end_of_the_word(str, i);
      } else if (str[i] == 'S') {
        result[j++] = 'S';  // asin
        i = end_of_the_word(str, i);
      } else if (str[i] == 'T') {
        result[j++] = 'T';  // atan
        i = end_of_the_word(str, i);
      }
    } else if (str[i] == 'S') {
      i++;
      if (str[i] == 'I') {
        result[j++] = 's';  // sin
        i = end_of_the_word(str, i);
      } else if (str[i] == 'Q') {
        result[j++] = 'q';  // sqrt
        i = end_of_the_word(str, i);
      }
    } else if (str[i] == 'T') {
      result[j++] = 't';  // tan
      i = end_of_the_word(str, i);
    } else if (str[i] == 'L') {
      i++;
      if (str[i] == 'N') {
        result[j++] = 'L';  // ln
        i = end_of_the_word(str, i);
      } else if (str[i] == 'O') {
        result[j++] = 'l';  // log
        i = end_of_the_word(str, i);
      }
    } else if (str[i] == 'C') {
      result[j++] = 'c';  // cos
      i = end_of_the_word(str, i);
    } else if (str[i] == 'X') {
      result[j++] = 'X';  // X

    }

    else if ((!(str[i] >= 'A' && str[i] <= 'z')) || str[i] == '^') {
      result[j++] = str[i];
    }
  }
  result[j++] = '\0';
  strcpy(str, result);
}

double execute(char op, double first, double second) {
  double res = 0;
  if (op == '+') {
    res = first + second;
  }  //	Сложение
  else if (op == '-') {
    res = first - second;
  }  //	Вычитание
  else if (op == '*') {
    res = first * second;
  }  //	Умножение
  else if (op == '/') {
    res = first / second;
    if (second == 0) {
      res = NAN;
    }
  }  //	Деление
  else if (op == '^') {
    res = pow(first, second);
  }  //	Степень
  else if (op == '%') {
    res = fmod(first, second);
  }  //	Остаток от деления
  return res;
}

double execute_unari(char op, double value) {
  double res = 0;
  if (op == 'c') {
    res = cos(value);
  }  //	cos
  else if (op == 'C') {
    res = acos(value);
  }  //	acos
  else if (op == 's') {
    res = sin(value);
  }  //	sin
  else if (op == 'S') {
    res = asin(value);
  }  //	asin
  else if (op == 't') {
    res = tan(value);
  }  //	tan
  else if (op == 'T') {
    res = atan(value);
  }  //	sin
  else if (op == 'L') {
    res = log(value);
  }  //	ln
  else if (op == 'l') {
    res = log10(value);
  }  //	log
  else if (op == 'q') {
    res = sqrt(value);
  }  // sqrt
  return res;
}

int end_of_the_word(char *str, size_t i) {
  for (; i < strlen(str);) {
    if (str[i + 1] >= 'A' && str[i + 1] <= 'z') {
      i++;
    } else {
      break;
    }
  }
  return i;
}

double stod(const char *s) {
  double rez = 0, fact = 1;
  if (*s == '-') {
    s++;
    fact = -1;
  }
  for (int point_seen = 0; *s; s++) {
    if (*s == '.') {
      point_seen = 1;
      continue;
    }
    int d = *s - '0';
    if (d >= 0 && d <= 9) {
      if (point_seen) fact /= 10.0f;
      rez = rez * 10.0f + (float)d;
    }
  }
  return rez * fact;
}

void delete_null(char *str) {
  int i = 0;
  for (; str[i]; ++i) {
  }
  --i;
  while (str[i] == '0' || str[i] == ',') {
    if (str[i] == ',') {
      str[i--] = 0;
      break;
    }
    str[i--] = 0;
  }
  str[++i] = 0;
}

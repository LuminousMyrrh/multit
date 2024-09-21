#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"

// a * b / (c - d)

int parse_expression(Parser *parser);

// Function to skip spaces in the source
void skip_spaces(Parser *parser) {
  while (isspace(parser->source[parser->curr_idx])) {
    parser->curr_idx++;
  }
}

// Function to parse a number
int parse_number(Parser *parser) {
  int result = 0;
  while (isdigit(parser->source[parser->curr_idx])) {
    result = result * 10 + (parser->source[parser->curr_idx] - '0');
    parser->curr_idx++;
  }
  return result;
}

// Function to parse a factor (number or expression in parentheses)
int parse_factor(Parser *parser) {
  skip_spaces(parser);

  if (parser->source[parser->curr_idx] == '(') {
    parser->curr_idx++; // Skip '('
    int result = parse_expression(parser);
    skip_spaces(parser);
    if (parser->source[parser->curr_idx] == ')') {
      parser->curr_idx++; // Skip ')'
    } else {
      fprintf(stderr, "Error: Mismatched parentheses\n");
      exit(EXIT_FAILURE);
    }
    return result;
  } else {
    return parse_number(parser);
  }
}

unsigned long long int power(int base, int exp) {
    unsigned long long int result = 1;
    while (exp != 0) {
        result *= base;
        --exp;
    }
    return result;
}

// Function to parse a term (factor followed by * or /)
int parse_term(Parser *parser) {
  int result = parse_factor(parser);

  while (1) {
    skip_spaces(parser);

    char op = parser->source[parser->curr_idx];
    if (op == '*' || op == '/' || op == '^') {
      parser->curr_idx++; // Skip operator
      int next_factor = parse_factor(parser);
      if (op == '*') {
        result *= next_factor;
      } else if (op == '/') { // op == '/'
        if (next_factor == 0) {
          fprintf(stderr, "Error: Division by zero\n");
          exit(EXIT_FAILURE);
        }
        result /= next_factor;
      } else {
        result = power(result, next_factor);
      }
    } else {
      break;
    }
  }

  return result;
}

// Function to parse an expression (term followed by + or -)
int parse_expression(Parser *parser) {
  int result = parse_term(parser);

  while (1) {
    skip_spaces(parser);

    char op = parser->source[parser->curr_idx];
    if (op == '+' || op == '-') {
      parser->curr_idx++; // Skip operator
      int next_term = parse_term(parser);
      if (op == '+') {
        result += next_term;
      } else { // op == '-'
        result -= next_term;
      }
    } else {
      break;
    }
  }

  return result;
}

// Main function to evaluate an expression
int solve(const char *expression) {
  Parser parser = {0, expression};
  int result = parse_expression(&parser);

  skip_spaces(&parser);

  if (parser.source[parser.curr_idx] != '\0') {
    fprintf(stderr, "Error: Unexpected characters at end of expression\n");
    exit(EXIT_FAILURE);
  }

  return result;
}

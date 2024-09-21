#ifndef EXPRESSION_H
#define EXPRESSION_H
// (1 + 10) - 90 + (80 - 1)

typedef struct Expression {
  struct Expression *left;
  struct Expression *right;
  int value;
  char oper;
} Expression;

typedef struct {
  int curr_idx;
  const char *source;
} Parser;

static Expression *parseExpr(Parser *p);

int solve(const char *expression);

#endif

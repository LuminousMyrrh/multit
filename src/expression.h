#ifndef EXPRESSION_H
#define EXPRESSION_H
// (1 + 10) - 90 + (80 - 1)

typedef struct {
  int curr_idx;
  const char *source;
} Parser;

int solve(const char *expression);

#endif

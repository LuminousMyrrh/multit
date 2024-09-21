#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expression.h"

static unsigned long long int conv_hex_to_dec(const char *hex) {
  unsigned long long int res = 0;
  size_t i = 0;
  size_t len = strlen(hex);

  // Check for "0x" prefix
  if (len > 1 && hex[0] == '0' && hex[1] == 'x') {
    i = 2; // Start conversion after "0x"
  }

  // Convert each character
  for (; i < len; i++) {
    char c = hex[i];
    int value;

    // Convert hex character to its decimal value
    if (isdigit(c)) {
      value = c - '0'; // Convert '0'-'9' to 0-9
    } else if (c >= 'A' && c <= 'F') {
      value = c - 'A' + 10; // Convert 'A'-'F' to 10-15
    } else if (c >= 'a' && c <= 'f') {
      value = c - 'a' + 10; // Convert 'a'-'f' to 10-15
    } else {
      printf("Invalid hexadecimal character: %c\n", c);
      return 0; // Return 0 on invalid input
    }

    res = res * 16 +
          value; // Shift left by multiplying by 16 and add current digit value
  }

  return res;
}

void addBinary(char *block, long long int up_to) {
  memset(block, '0', 4); // Reset block to all '0's

  // Convert up_to (decimal) to binary in block (4 bits)
  for (int i = 3; i >= 0; i--) {
    block[i] = (up_to & 1) ? '1' : '0'; // Set each bit
    up_to >>= 1;                        // Shift right to process the next bit
  }
}

static char *conv_hex_to_bin(const char *hex) {
  // 3fa => 0011 1111 1010

  size_t i = 0;
  size_t len = strlen(hex);

  // Check for "0x" prefix
  if (len > 1 && hex[0] == '0' && hex[1] == 'x') {
    i = 2; // Start conversion after "0x"
  }

  size_t output_len = len * 5;

  char *res = (char *)malloc(sizeof(char) * output_len);
  if (!res)
    return NULL;

  char block[5];
  int inner_idx = 0;

  for (; i < len; i++) {
    char c = hex[i];
    long long int up_to = 0;

    // Convert hex character to its decimal value
    if (isdigit(c)) {
      up_to = c - '0'; // Convert '0'-'9' to 0-9
    } else if (c >= 'A' && c <= 'F') {
      up_to = c - 'A' + 10; // Convert 'A'-'F' to 10-15
    } else if (c >= 'a' && c <= 'f') {
      up_to = c - 'a' + 10; // Convert 'a'-'f' to 10-15
    } else {
      free(res);
      return NULL;
    }

    addBinary(block, up_to);
    memcpy(res + inner_idx, block, 4);
    inner_idx += 4;

    if (i < len - 1) {
      res[inner_idx++] = ' ';
    }
  }
  res[inner_idx] = '\0';
  return res;
}

static char *conv_dec_to_hex(const char *dec) {
  // Convert decimal string to unsigned long long
  unsigned long long decimal = strtoull(dec, NULL, 10);
  if (decimal == 0) {
    return "0x0";
  }

  // Calculate maximum length needed for hexadecimal representation
  size_t hex_length = sizeof(decimal) * 2 +
                      3; // 2 chars per byte + "0x" prefix + null terminator
  char *res = (char *)malloc(hex_length);
  if (res == NULL) {
    return NULL; // Handle memory allocation failure
  }

  res[0] = '0';
  res[1] = 'x';
  size_t idx = 2;

  // Convert decimal to hexadecimal
  while (decimal > 0) {
    int rem = decimal % 16;

    if (rem < 10) {
      res[idx++] = rem + '0';
    } else {
      res[idx++] = rem - 10 + 'a';
    }

    decimal /= 16;
  }

  // Reverse the string to get the correct order
  for (size_t i = 2, j = idx - 1; i < j; i++, j--) {
    char temp = res[i];
    res[i] = res[j];
    res[j] = temp;
  }

  res[idx] = '\0'; // Null-terminate the string

  return res;
}

static char *conv_dec_to_bin(const char *dec) {
  // 69 => 0110 1001

  size_t len = strlen(dec);
  size_t output_len =
      len * 5 + 1; // Adjusted size for spaces and null terminator

  char *res = (char *)malloc(sizeof(char) * output_len);
  if (!res)
    return NULL;

  char block[4];
  int inner_idx = 0;

  for (size_t i = 0; i < len; i++) {
    char c = dec[i];
    unsigned long long up_to = c - '0';

    addBinary(block, up_to);
    memcpy(res + inner_idx, block, 4);
    inner_idx += 4;

    if (i < len - 1) {
      res[inner_idx++] = ' '; // Add space between binary numbers
    }
  }
  res[inner_idx] = '\0'; // Null-terminate the result string
  return res;
}

void repl() {
    char line[256];
    printf("Type 'q' to quit.\n");

    while (1) {
        printf(">> ");
        if (fgets(line, sizeof(line), stdin) == NULL) {
            perror("Error reading input");
            continue;
        }

        line[strcspn(line, "\n")] = 0;

        if (strcmp(line, "q") == 0) {
            printf("Exiting REPL.\n");
            break;
        }

        int res = solve(line);
        if (res == -1) {
            printf("Invalid input.\n");
        } else {
            printf("%d\n", res);
        }
    }
}

void hex(const char *sub, const char *val) {
    if (strncmp(sub, "dec", 3) == 0) {
      printf("----------------\n");
      printf("%s => \033[32m%llu\033[0m\n", val, conv_hex_to_dec(val));
      printf("----------------\n");
      return;

    } else if (strncmp(sub, "bin", 3) == 0) {
      char *res = conv_hex_to_bin(val);
      printf("----------------\n");
      printf("%s => \033[32m%s\033[0m\n", val, res);
      printf("----------------\n");
      free(res);
      return;
    } else {
      return;
    }
}

void dec(const char *sub, const char *val) {
    if (strncmp(sub, "hex", 3) == 0) {
      char *res = conv_dec_to_hex(val);
      printf("----------------\n");
      printf("%llu => \033[32m%s\033[0m\n", strtoull(val, NULL, 10), res);
      printf("----------------\n");
      free(res);
      return;
    } else if (strncmp(sub, "bin", 3) == 0) {
      printf("----------------\n");
      char *res = conv_dec_to_bin(val);
      printf("%llu => \033[32m%s\033[0m\n", strtoull(val, NULL, 10), res);
      printf("----------------\n");
      free(res);
      return;
    } else {
      return;
    }
  
}

int main(int argc, char **argv) {
  if (argc == 1) {
    printf("Usage: <program> <source_base> <target_base> <value>\n");
    return 69;
  }

  if (strncmp(argv[1], "repl", 4) == 0) {
    repl();
    return 0;
  } else if (strncmp(argv[1], "hex", 3) == 0) {
    if (argc != 4) return 69;
    hex(argv[2], argv[3]);
    return 0;

  } else if (strncmp(argv[1], "dec", 3) == 0) {
    if (argc != 4) return 69;
    dec(argv[2], argv[3]);
    return 0;

  } else if (strncmp(argv[1], "count", 5) == 0) {
    char line[256];
    printf("Enter string: ");
    fgets(line, sizeof(line), stdin);
    line[strcspn(line, "\n")] = 0;
    printf("=> \033[32m%lu\033[0m\n", strlen(line));
    return 0;
  }
  return 0;
}

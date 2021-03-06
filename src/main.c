#include <stdio.h>

#include "builtin_functions.h"
#include "debug.h"
#include "parser.h"

void read_line(FILE *buffer) {
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, buffer)) != -1) {

    FILE *line_buffer = fmemopen(line, strlen(line), "r");
    init_lexer(line_buffer);

    SQD3_OBJECT *result = expr();

    printf("%lld \n", read_integer_from_object(result));
  }
}

void read_file(const char **argv) {

  FILE *buffer = fopen(argv[1], "r");

  read_line(buffer);

  fclose(buffer);
}

void repl() {
  for (;;) {

    printf("\n> ");
    char input[120];
    fgets(input, 120, stdin);

    FILE *buffer = fmemopen(input, strlen(input), "r");

    init_lexer(buffer);
    SQD3_OBJECT *result = expr();
    printf("%lld \n", read_integer_from_object(result));
  }
}

int main(int argc, const char  **argv) {

  init_vtable();

  SQD3_OBJECT *print_ref = build_builtin_function_ref("print", &print_function);
  declare_local_variable("print", print_ref);

  argc > 1 ? read_file(argv) : repl();

  return 0;
}

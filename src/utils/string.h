#ifndef STRING
#define STRING

#include <stdio.h>

typedef struct {
  char *data;
  size_t length;
} my_string;

my_string cstr_to_my_string(char *str);
void my_string_free(my_string *s);

#endif // !STRING

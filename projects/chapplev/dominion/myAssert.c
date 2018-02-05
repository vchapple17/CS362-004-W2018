#include <stdio.h>
#include <string.h>
#include "myAssert.h"

int myAssert(int conditional, char * name, char * T, char * F) {
  if (conditional) {
    printf("%s\t%s\n", T, name);
  }
  else {
    printf("%s\t%s\n", F, name);
  }
  return 0;
}

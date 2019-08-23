#include <stdio.h>
#include <bar.h>

#include "local.h"

int main(int argc, char *argv[])
{
   printf("Hello word\n");
   printf("  function1(10) -> %d\n", function1(10));
   printf("  function2(\"hello\") -> %d\n", function2("hello"));
   printf("  function3(10) -> %d\n", function3(10));
   return 0;
}

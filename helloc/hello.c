#include <stdio.h>
#define swap(a,b) {auto t = a; a = b; b = t;}
 
int main()
{
  int a = 1;
  int b = 2;
  swap(a,b);
  printf("a = %d, b = %d\n", a, b);

  return 1;
}

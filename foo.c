#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
  char foo[20];
  char foo2[20];
  char *din;
  scanf("%s", foo);
  scanf("%s", foo2);
  printf("%s\n",foo );
  printf("%s\n",foo2 );
  din = malloc(sizeof(char)*strlen(foo));
  sscanf(foo, "%s", din);
  printf("%s \ndim din: %li \ndim foo: %li",din, sizeof(din), sizeof(foo));
}

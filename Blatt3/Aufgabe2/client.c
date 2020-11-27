#include "rpc/rpc.h"
#include "maths.h"

int main() {
  intpair numbers[2] = {2,6};
  CLIENT *c1 = clnt_create("localhost", MATHPROG, MATHVERS, "tcp");
  if(c1<0) {
    perror("Fehler beim Client\n");    
  }
  int *erg = add_1(numbers, c1);
  printf("%d, %d\n", erg[0], erg[1]);
}

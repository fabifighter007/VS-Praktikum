#include "rpc/rpc.h"
#include "maths.h"

int main() {
  intpair numbers[2] = {2,6};
  clnt_create("localhost", MATHPROG, MATHVERS, "tcp");
  int *erg = add_1(&numbers, c1);
  printf("%d, %d", erg[0], erg[1]);
}

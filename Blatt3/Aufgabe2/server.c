#include "rpc/rpc.h"
#include "maths.h"


int *add_1_svc(intpair* pair, struct svc_req* rqstp) {
  int *res;
  *res = pair->a + pair->b;
  return res;
}

int *multiply_1_svc(intpair* pair, struct svc_req* rqstp) {
  int *res;
  *res = pair->a * pair->b;
  return res;
}


int *cube_1_svc(int * pair, struct svc_req* rqstp) {
  int *res;
  *res = (*pair) * (*pair) * (*pair);
  return res;
}

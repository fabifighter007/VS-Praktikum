#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void bereich(int);

void myFork(int num) {
  int z = fork();
  switch(z) {
    case 0: printf("Ich bin Kind %d mit der PID %d \n", num, getpid()); bereich(num); exit(0); break; //sohn
    case -1: perror("Fehler bei fork \n"); exit(1); break; //fehler
    default: break; //elter
  }
}

void bereich(int i) {
  for(int z=0;z<3;z++) {
    printf("Prozess %d betritt kritischen Bereich \n", i);
    sleep(1);
    printf("Prozess %d verlaesst kritischen Bereich \n", i);
    sleep(1);
  }
}

int main() {
  for(int i=1; i<4;i++) {
    myFork(i);
  }
  return 0;
}

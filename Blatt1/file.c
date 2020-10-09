#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

void bereich(int i);

void myFork() { 
	// child process because return value zero 
	
  int z = fork();
        if (z == 0) {
          printf("Child 1: %d", getpid());
          printf("\n");
          bereich(1);
        } else if(z == -1) {
          perror("Fehler");
          exit(1);
        } else {
            z = fork();
            if(z == 0) {
              printf("Child 2: %d", getpid());
              printf("\n");
              bereich(2);
            }  else if(z == -1) {
              perror("Fehler");
              exit(1);
            } else {
                 z = fork();
                if(z == 0) {
                  printf("Child 3: %d", getpid());
                  printf("\n");
                  bereich(3);
                } else if(z == -1) {
                  perror("Fehler");
                  exit(1);
                }
            }
        }
}

void bereich(int i) {
  for(int z=0; z<30; z++) {
    printf("Prozess %d betritt kritischen Bereich\n", i);
    sleep(1);
    printf("Prozess %d verlaesst kritischen Bereich\n", i);
    sleep(1);
  }
}

int main() { 
	myFork(); 
	return 0; 
} 


#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>

#include "sys/msg.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void bereich(int i);
int semget(key_t key, int nsems, int semflg);
void P(int);
void V(int);

  key_t sem_key;
  int sem_id;

void myFork() { 
  // child process because return value zero 

  int z = fork();
  if (z == 0) {
    printf("Child 1: %d\n", getpid());
      for(int z=0; z<3; z++) {
        P(1);
        printf("Prozess 1 betritt kritischen Bereich\n");
        sleep(1);
        V(1);
        printf("Prozess 1 verlaesst kritischen Bereich\n");
        sleep(1);
      } 
  } else if(z == -1) {
    perror("Fehler");
    exit(1);
  } 


  z = fork();
  if(z == 0) {
    printf("Child 2: %d\n", getpid());
      for(int z=0; z<3; z++) {
        P(1);
        printf("Prozess 2 betritt kritischen Bereich\n");
        sleep(1);
        V(1);
        printf("Prozess 2 verlaesst kritischen Bereich\n");
        sleep(1);
      } 
  }  else if(z == -1) {
    perror("Fehler");
    exit(1);
  } 


  z = fork();
  if(z == 0) {
    printf("Child 3: %d\n", getpid());
      for(int z=0; z<3; z++) {
        P(1);
        printf("Prozess 3 betritt kritischen Bereich\n");
        sleep(1);
        V(1);
        printf("Prozess 3 verlaesst kritischen Bereich\n");
        sleep(1);
      }    
  } else if(z == -1) {
    perror("Fehler");
    exit(1);
  }
}

void bereich(int i) {
  for(int z=0; z<3; z++) {
    printf("Prozess 1 betritt kritischen Bereich\n");
    sleep(1);
    printf("Prozess 1 verlaesst kritischen Bereich\n");
    sleep(1);
  }
}

void init_sem() {
if(semctl(sem_id, 0, SETVAL, 1) < 0) {
    perror("Fehler in semctl");
    exit(1);
    }
}

void P(int sem_num) {
  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_op = -1;
  semaphore.sem_flg =~ (IPC_NOWAIT|SEM_UNDO);
  if(semop(sem_id,&semaphore, 1)) {
    perror("Fehler in semop P()\n");
    exit(1);
  }
}

void V(int sem_num) {
  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_op = 1;
  semaphore.sem_flg =~ (IPC_NOWAIT|SEM_UNDO);

    if(semop(sem_id, &semaphore, 1)) {
    perror("Fehler in semop V()\n");
    exit(1);
  }
}


int main() { 
  if((sem_key = ftok("/home/lars/Dokumente/VS-Praktikum/Blatt1/file.c", '1')) < 0) {
    perror("Fehler in ftok");
    exit(1);
  }

  if((sem_id = semget(sem_key, 1, IPC_CREAT|0666)) < 0) {
    perror("Fehler in semget");
    exit(1);
  }

  init_sem();
  myFork(); 


  return 0; 
} 


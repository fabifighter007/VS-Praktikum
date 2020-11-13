#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "sys/stat.h"



key_t sem_key;
int sem_id;

void bereich(int);
void myFork(int);

void init_sem() {
  if(semctl(sem_id, 0, SETVAL, 1) <0) {
    perror("Fehler bei semctl \n");    
    exit(1);
  }
}

void P(int sem_num) {
  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_op = -1; // P -> -1
  semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  if(semop(sem_id, &semaphore, 1)) {
    perror("Fehler bei P() \n");
    exit(1);
  }
}

void V(int sem_num) {
  struct sembuf semaphore;
  semaphore.sem_num = 0;
  semaphore.sem_op = 1; // V -> 1
  semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  if(semop(sem_id, &semaphore, 1)) {
    perror("Fehler bei V() \n");
    exit(1);
  }
}

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
    P(1);
    printf("Prozess %d betritt kritischen Bereich \n", i);
    sleep(1);

    V(1);
    printf("Prozess %d verlaesst kritischen Bereich \n", i);
    sleep(1);
  }
}

int main() {
  if((sem_key = ftok("/home/lars/Dokumente/VS-Praktikum/Blatt1/Aufgabe2.c", '1')) < 0) {
    perror("Fehler bei ftok \n");
    exit(1);
  }

  if((sem_id = semget(sem_key, 1, IPC_CREAT | 0666)) < 0) { 
    perror("Fehler bei semget \n");
    exit(1);
  }
  
  init_sem();


  for(int i=1; i<4;i++) {
    myFork(i);
  }
  return 0;
}

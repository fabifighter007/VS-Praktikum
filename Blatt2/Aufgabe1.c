#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "sys/stat.h"


key_t sem_key;
int sem_id;
int nextGabel(int);

typedef enum status {DENKEN, ESSEN} STATUS;

typedef struct person {
  int id;
  enum status status;
} person;


void P(int sem_num) {
  struct sembuf semaphore;
  semaphore.sem_num = sem_num;
  semaphore.sem_op = -1; // P -> -1
  semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  if(semop(sem_id, &semaphore, 1)) {
    perror("Fehler bei P() \n");
    exit(1);
  }
}

void V(int sem_num) {
  struct sembuf semaphore;
  semaphore.sem_num = sem_num;
  semaphore.sem_op = 1; // V -> 1
  semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  if(semop(sem_id, &semaphore, 1)) {
    perror("Fehler bei V() \n");
    exit(1);
  }
}

void sperreGabel(int id) {
  printf("Sperre %d \n", id);
  P(id);
  P(nextGabel(id));
}

void freigebenGabel() {
  printf();
}

int nextGabel(int id) {
  return (id+1)%5;
}

void init_sem() {
  for(int z=0;z<5;z++) {
    if(semctl(sem_id, z, SETVAL, 1) <0) {
      perror("Fehler bei semctl \n");    
      exit(1);
    }
  }
}

int main(){
    if((sem_key = ftok("/home/lars/Dokumente/VS-Praktikum/Blatt2/Aufgabe1.c", '1')) < 0) {
    perror("Fehler bei ftok \n");
    exit(1);
  }
  if((sem_id = semget(sem_key, 5, IPC_CREAT | 0666)) < 0) { 
    perror("Fehler bei semget \n");
    exit(1);
  }
  
  init_sem();

  person person[5];
  for(int i=0;i<5;i++) {
    person[i].id = i;
    person[i].status = DENKEN;
  }
  sperreGabel(2);
}

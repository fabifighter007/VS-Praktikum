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
} person_t;

person_t person[5];

void neuerStatus(int i) {
  if(person[i].status == 0) {
    person[i].status = 1;
  } else {
    person[i].status = 0;
  }
}

void palast() {
  while(1) {
    time_t t;
//    srand((unsinged) time(t));
    
    int r = rand()%5;
    if(person[r].status == DENKEN) {
      printf("Person %d möchte essen", r);
      person[r].status = ESSEN;
    } else {
      printf("Person %d möchte DENKEN", r);
      person[r].status = DENKEN;
    }

/*    sperreGabel(r);
    neuerStatus(r);
    sleep(1);
    V(r);
  */
    }
}

void P(int sem_num) {
  struct sembuf semaphore[2];
  semaphore[0].sem_num = sem_num;
  semaphore[0].sem_op = -1; // P -> -1
  semaphore[0].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  semaphore[1].sem_num = nextGabel(sem_num);
  semaphore[1].sem_op = -1; // P -> -1
  semaphore[1].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  if(semop(sem_id, semaphore, 2)) {
    perror("Fehler bei P() \n");
    exit(1);
  }
}

void V(int sem_num) {
  struct sembuf semaphore[2];
  semaphore[0].sem_num = sem_num;
  semaphore[0].sem_op = 1; // V -> 1
  semaphore[0].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  semaphore[1].sem_num = nextGabel(sem_num);
  semaphore[1].sem_op = 1; // V -> 1
  semaphore[1].sem_flg = ~(IPC_NOWAIT|SEM_UNDO);
  
  if(semop(sem_id, semaphore, 2)) {
    perror("Fehler bei V() \n");
    exit(1);
  }
}

void sperreGabel(int id) {
  printf("Sperre %d \n", id);
  P(id);
  P(nextGabel(id));
}

void freigebenGabel(int id) {
  printf("Gebe Gabel frei %d \n", id);
  V(id);
  P(nextGabel(id));
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

  for(int i=0;i<5;i++) {
    person[i].id = i;
    person[i].status = DENKEN;
  }
  sperreGabel(2);
  freigebenGabel(2);
  palast();
}

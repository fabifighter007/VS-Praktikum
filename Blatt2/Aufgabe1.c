#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "sys/stat.h"

key_t sem_key;
int sem_id;
int nextGabel(int);
void P(int);
void V(int);


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
    printf("Betrete Palast\n");
    time_t t;
    srand((unsigned) time(&t));
    while(1) {
    printf("Starten\n");
    
    int r = rand()%5;
    printf("Random %d \n", r);
    
    if(person[r].status == DENKEN) {
      printf("Person %d möchte essen \n", r);
      P(r+1);
      person[r].status = ESSEN;
    } else {
      printf("Person %d möchte DENKEN \n", r);
      V(r+1);
      person[r].status = DENKEN;
    }
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
  for(int zz=0;zz<5;zz++) {
      printf("Zahl: %d \n",zz);
    if(semctl(sem_id, zz, SETVAL, 1) < 0) {
      perror("Fehler bei semctl \n");    
      exit(1);
    }
  }
  printf("initsem() fertig\n");
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
  
  printf("initsem()\n");
  init_sem();

  for(int i=0;i<5;i++) {
    person[i].id = i;
    person[i].status = DENKEN;
  }

  palast();
}

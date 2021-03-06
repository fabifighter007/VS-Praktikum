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

int philID;

typedef enum status {DENKEN, ESSEN} STATUS;


typedef struct person {
  int id;
  enum status status;
} person_t;

int randZahl() {
  time_t t;
  srand((unsigned) time(&t));
  return (rand()%4)+philID; 
}

void palast() {
  int j = 0;
  while(j<5) {
    j++;

    P(philID);
    printf("Philisoph %d geht zum essen.\n", philID);
    sleep(randZahl());
    V(philID);
    printf("Philisoph %d denkt wieder.\n", philID);
    sleep(randZahl());
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

  for(int u=0;u<5;u++) {
    int z = fork();
    switch(z) {
      case 0: printf("Ich bin Philosoph %d mit der PID %d \n", u, getpid()); philID=u; palast(); exit(0); break; //sohn
      case -1: perror("Fehler bei fork \n"); exit(1); break; //fehler
      default: break; //elter
    }
  }
}

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
void P(int);
void V(int);

void lesezugriff() {
  for(int i=0; i<3;i++) {
    //zugriff auf var - kritischer bereich betreten
    printf("Leser betritt bereich\n");
    sleep(1);
    //bereich verlassen
    printf("Leser verlaesst bereich\n");
    sleep(1);
  }
}

void schreibzugriff() {
  for(int i=0; i<3; i++) {
    //betreten
    printf("Schreiber betritt bereich\n");
    sleep(1);
    //verlassen
    printf("Schreiber verlaesst bereich\n");
    sleep(1);
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
  semaphore.sem_num = 0; //semnum alt
  semaphore.sem_op = 1; // V -> 1
  semaphore.sem_flg = ~(IPC_NOWAIT|SEM_UNDO);

  if(semop(sem_id, &semaphore, 1)) {
    perror("Fehler bei V() \n");
    exit(1);
  }
}

void init_sem() {
  for(int zz=0;zz<5;zz++) {
    if(semctl(sem_id, zz, SETVAL, 1) < 0) {
      perror("Fehler bei semctl \n");    
      exit(1);
    }
  }
  printf("initsem() fertig\n");
}

int main(){
  if((sem_key = ftok("/home/lars/Dokumente/VS-Praktikum/Blatt3/Aufgabe.c", '1')) < 0) {
    perror("Fehler bei ftok \n");
    exit(1);
  }
  if((sem_id = semget(sem_key, 5, IPC_CREAT | 0666)) < 0) { 
    perror("Fehler bei semget \n");
    exit(1);
  }

  printf("initsem()\n");
  init_sem();

  for(int u=0;u<7;u++) {
    int z = fork();
    if(u>=5) {
      switch(z) {
        case 0: printf("Ich bin ein Schreiber %d mit der PID %d \n", u, getpid()); lesezugriff(); exit(0); break; //sohn
        case -1: perror("Fehler bei fork \n"); exit(1); break; //fehler
        default: break; //elter
      }
    } else {
      switch(z) {
        case 0: printf("Ich bin ein Leser %d mit der PID %d \n", u, getpid()); schreibzugriff(); exit(0); break; //sohn
        case -1: perror("Fehler bei fork \n"); exit(1); break; //fehler
        default: break; //elter
      }
    }
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "sys/types.h"
#include "sys/ipc.h"
#include "sys/sem.h"
#include "sys/stat.h"


key_t sem_key;
int sem_id;


typedef enum status {DENKEN, ESSEN} STATUS;

typedef struct person {
  int id;
  enum status status_status;
} person;

void init_sem() {
  if(semctl(sem_id, 0, SETVAL, 1) <0) {
    perror("Fehler bei semctl \n");    
    exit(1);
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
}

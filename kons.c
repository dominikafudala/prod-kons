#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include "operacje.h"


struct bufor{
	int mtype;
	int mvalue;
};

int *pam; 
#define MAX2 12
#define MAX 10
#define PELNY 2
#define PUSTY 1
#define zapis pam[MAX+1]
#define odczyt pam[MAX]

int main()
{
   key_t klucz, kluczm, kluczsem;
   int msgID, shmID, semID;
   int i;
   struct bufor komunikat;

   //printf("konsument--------------------------------\n");


//uzyskanie dosepu do kolejki komunikatow
if ( (klucz = ftok(".", 'A')) == -1 )
{
	printf("Blad ftok (kons)\n");
	exit(1);
}
msgID=msgget(klucz,IPC_CREAT|0666); //tworzenie kk
if (msgID==-1)
	{printf("blad kolejki komunikatow\n"); exit(1);}
 
//uzyskanie dosepu do pamieci dzielonej
kluczm=ftok(".",'B');
shmID=shmget(kluczm, MAX2*sizeof(int), IPC_CREAT|0666);//tworzenie pam. dz

//semafory
kluczsem=ftok(".",'C');
   semID= semget(kluczsem, 1, IPC_CREAT | 0666);
   if ( semID == -1)
   {
      perror("Blad semget ");
      exit(1);
   }

//przylaczenie pam. dzielonej-- shmat   
pam = (int*)shmat(shmID, NULL, 0);


//odbieranie/wysylanie odpowiednich komunikatow +
// odczyt z bufora  elementu o  indeksie odczyt (pam. dzielona)
msgrcv(msgID, &komunikat, sizeof(komunikat.mvalue), PELNY, 0);
printf("KONSUMENT odebrano: (1 - PUSTY 2 - PELNY) %d\n", komunikat.mtype);

//sekcja krytyczna -- semafor -- operacje na pamięci dzielonej
waitSem(semID, 0, 0);


printf("KONSUMENT pam[%d]: %d\n", odczyt, pam[odczyt]);

//koniec sekcji krytycznej
signalSem(semID, 0);

komunikat.mtype=PUSTY;
msgsnd(msgID,&komunikat,sizeof(komunikat.mvalue),0);


//modyfikacja indeksu do odczytu
odczyt = (odczyt+1)%MAX;


// //msgrcv -- odbiór komunikatu 
//odlaczanie pamieci dzielonej
//shmdt(pam);
exit(0);
}


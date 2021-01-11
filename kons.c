#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>


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
   key_t klucz, kluczm;
   int msgID, shmID;
   int i;
   struct bufor komunikat;

   printf("konsument--------------------------------\n");


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
shmID=shmget(kluczm, MAX2*sizeof(int), IPC_CREAT|0666);//tworzenie pam. dz.

//przylaczenie pam. dzielonej-- shmat   
pam = (int*)shmat(shmID, NULL, 0);

//sekcja krytyczna -- semafor -- operacje na pamięci dzielonej
//odbieranie/wysylanie odpowiednich komunikatow +
// odczyt z bufora  elementu o  indeksie odczyt (pam. dzielona)
msgrcv(msgID, &komunikat, sizeof(komunikat.mvalue), PELNY, 0);
printf("KONSUMENT odebrano: %d\n", komunikat.mtype);

printf("KONSUMENT pam[%d]: %d\n", odczyt, pam[odczyt]);

komunikat.mtype=PUSTY;
msgsnd(msgID,&komunikat,sizeof(komunikat.mvalue),0);

//modyfikacja indeksu do odczytu
odczyt = (odczyt+1)%MAX;

//msgrcv -- odbiór komunikatu 

}


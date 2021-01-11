#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

struct bufor{
	int mtype;
	int mvalue;
};

int *pam;

#define MAX 10
#define MAX2 12
#define PELNY 2
#define PUSTY 1
#define SK 3
#define odczyt pam[MAX]
#define zapis pam[MAX+1]
int main()
{
   key_t klucz, kluczm;
   int msgID;
   int shmID;
   int i; // to, co ma trafic do bufora 
   time_t czas;   
   struct bufor komunikat;
   pid_t pidNr;

printf("producent--------------------------------\n");

//uzyskanie dostepu do kolejki komunikatow
if ( (klucz = ftok(".", 'A')) == -1 )
{
	printf("Blad ftok (prod)\n");
	exit(1);
}
msgID=msgget(klucz,IPC_CREAT|0666); //tworzenie kk
if (msgID==-1)
	{printf("blad kolejki komunikatow\n"); exit(1);}

//uzyskanie dostepu do pamieci dzielonej
kluczm=ftok(".",'B');
shmID=shmget(kluczm, MAX2*sizeof(int), IPC_CREAT|0666);//tworzenie pam. dz.

//przylaczenie pamieci dzielonej
pam = (int*)shmat(shmID, NULL, 0);

//wysylanie/odbieranie odpowiednich komunikatow
msgrcv(msgID, &komunikat, sizeof(komunikat.mvalue), PUSTY, 0);

//operacje na pamieci dzielonej w sekcji krytycznej -- semafory

//produkcja - dodanie rekordu do puli buforow  pod indeks - zapis  -- getpid()
pidNr=getpid();
pam[zapis] = pidNr;
printf("PRODUCENT pam[%d]: %d\n", zapis, pam[zapis]);

//wyslanie odpowiedniego komunikatu
komunikat.mtype=PELNY;
msgsnd(msgID,&komunikat,sizeof(komunikat.mvalue),0);
 
//modyfikacja indeksu do zapisu
zapis = (zapis+1)%MAX;	
	

}


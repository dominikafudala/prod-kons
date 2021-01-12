#include <sys/msg.h>
#include <sys/shm.h>
#include "operacje.h"

int waitSem(int semID, int number, int flags) {
    struct sembuf operacje[1];
    operacje[0].sem_num = number;
    operacje[0].sem_op = -1;
    operacje[0].sem_flg = 0 | flags; //SEM_UNDO;

    if ( semop(semID, operacje, 1) == -1 )
    {
        //perror("Blad semop (waitSemafor)");
        return -1;
    }

    return 1;
}

void signalSem(int semID, int number) {
    struct sembuf operacje[1];
    operacje[0].sem_num = number;
    operacje[0].sem_op = 1;

    if (semop(semID, operacje, 1) == -1)
        perror("Blad semop (postSemafor): ");
}
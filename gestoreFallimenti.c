#include <stdio.h>
#include <signal.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include <sys/types.h>

#define NUM_PFC 3

typedef struct PFC{

int pid;

}PFC;

PFC * extractPFC(PFC * PFC1, PFC * PFC2, PFC * PFC3);

void sendSignals();

int main(){

PFC PFC1, PFC2, PFC3;

PFC *choosenPFC;
//Provvisorio

scanf("%d", &PFC1.pid);
scanf("%d", &PFC2.pid);
scanf("%d", &PFC3.pid);

//Provvisorio

while(1){

choosenPFC = extractPFC(&PFC1, &PFC2, &PFC3);

printf("\nPID DEL PFC ESTRATTO: %d", (*choosenPFC).pid);

sleep(2);

sendSignals((*choosenPFC).pid);

}
}

PFC * extractPFC(PFC * PFC1, PFC * PFC2, PFC * PFC3){

    PFC *choosenPFC;

    int extractPFC;
    srand(time(NULL));
    extractPFC = rand()%3;

switch(extractPFC){

    case 0:
    choosenPFC = PFC1;
    break;

    case 1:
    choosenPFC = PFC2;
    break;

    case 2:
    choosenPFC = PFC3;
    break;

}

return choosenPFC;
}

void sendSignals(int PFC){

    int randomNumber = rand()%100;
    printf("\n%d",randomNumber);

    if(randomNumber == 1){

        kill(PFC, SIGSTOP);
        printf("\nSIGSTOP");

    }

    randomNumber = rand()%10000;
    printf("\n%d",randomNumber);

    if(randomNumber == 1){

        kill(PFC, SIGINT);
        printf("\nSIGINT");

    }

    randomNumber = rand()%10;
    printf("\n%d",randomNumber);

    if(randomNumber == 1){

        kill(PFC, SIGCONT);
        printf("\nSIGCONT");

    }

    randomNumber = rand()%10;
    printf("\n%d",randomNumber);

    if(randomNumber == 1){

        kill(PFC, SIGURS1);
        printf("\nSIGUSR1");

    }
}

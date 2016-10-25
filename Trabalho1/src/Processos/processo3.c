#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>

int main (void)
{
    int i;
    int segmento;
    int * waitingslots;
    char nome [100];
    
    printf("Digite seu nome:\n");

	segmento = shmget(8752, sizeof(int) * 10, IPC_CREAT | 0666);

	waitingslots = (int *) shmat(segmento, NULL, 0);

	for (int i = 0; i < 10; ++i)
	{
		if(waitingslots[i] == 0)
		{
			waitingslots[i] = getpid();
			break;
		}
	}

	sleep(4);

	waitingslots = (int *) shmat(segmento, NULL, 0);

	for (int i = 0; i < 10; ++i)
	{
		if(waitingslots[i] == 0)
		{
			waitingslots[i] = -getpid();
			break;
		}
	}

	printf("Ola mundo!!! \n");

    return 0;
}

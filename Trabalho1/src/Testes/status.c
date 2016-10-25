#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


#define EVER ;;

int main (void)
{
	int pid;
	int status;


	pid = fork();

	if (pid == 0)
	{
		execve("io", NULL, NULL);
	}
	else 
	{
		for(EVER)
		{

			sleep(1);
			waitpid(pid, &status, WNOHANG);
			printf("Status do processo filho: %d\n", status);
		}
	}

	return 0;
}
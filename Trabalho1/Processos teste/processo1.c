#include <stdio.h>


int main(int argc, char const *argv[])
{
	
	int i = 0;

	printf("Processo1: Iniciando processo1\n");

	for (int i = 0; i < 10; ++i)
	{
		sleep(2);
	}

	printf("Processo1: Encerrando processo1\n");

	return 0;
}
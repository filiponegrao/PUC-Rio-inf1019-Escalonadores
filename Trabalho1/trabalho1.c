#include "trabalho1.h"

/**
 * Processo
 */
struct process
{
 	int		pid;	/* Identificacao do processo*/
 	char *	name;	/* Nome do processo */
 	int		param;	/* Parametro generico. */
 	int		done;	/* Representa um boolean */

};

int main(int argc, char* argv[])
{
	//Testando
	Process * teste = (Process *)malloc(sizeof(Process));
	teste->name = "teste";
	teste->param = 0;

	executeRoundRobin(teste);

	return 0;
}


/***********************************/
/**** Escalonador Round-Robin ******/
/***********************************/
void executeRoundRobin(Process * p)
{
	//Testando
	printf("%s Prioridade: %d\n", p->name, p->param);
}


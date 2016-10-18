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

/***********************************/
/**   Variaveis de controle       **/
/***********************************/

/* Processos */
Process ** processes;

/* Numero de processos */
int nProcessess;

/* Numero de processos finalizados*/
int doneProcesses;

/***********************************/
/** Fim das variaveis de controle **/
/***********************************/


int main(int argc, char* argv[])
{
	//Testando
	Process * teste = (Process *)malloc(sizeof(Process));
	teste->name = "teste";
	teste->param = 0;


	return 0;
}


/***********************************/
/**** Escalonador Round-Robin ******/
/***********************************/
void executeRoundRobin()
{
	int i = 0;
	int index;
	Process * proc;

	//	Executa at'e que todos os processos tenham terminado
	while(doneProcesses < nProcessess)
	{
		//	Como o i 'e incrementado ate que todos os
		//	processos tenham sido executados por completo,
		//	ele pode assumir um valor que tende a infinito.
		//	Entao devemos pegar apenas um index dentro do dominio
		//	(de 0 ate nProcessess).
		index = i % nProcessess;

		//	Recuperamos um processo
		proc = processes[index];

		//	Primeiro verificamos se o processo ja foi iniciado.
		//	No caso de nao ter sido:
		if(proc->pid == 0)
		{
			proc->pid = fork();

			//	Processo pai
			if(proc->pid != 0)
			{
				//	Processo pai aguarda a fatia de tempo
				//	necessaria.
				sleep(TIMESLICE);

				//	E entao interrompe o processo filho.
				printf("Interrompendo processo %d, de nome: %s e pid: %d\n", index, proc->name, proc->pid);
				kill(proc->pid, SIGSTOP);

			}
			else
			{
				//	Processo filho executa o programa em
				//	questao sem parametros no caso.
				printf("Executando processo %d, de nome: %s e pid: %d\n", index, proc->name, proc->pid);
				execve(proc->name, NULL, NULL);
			}
		//	No caso de ja ter sido incializado
		}
		else
		{
			//	Verifica se o processo em questao
			//	ja teve sua execucao finalizada.
			//	Se ainda nao terminou:
			if(proc->terminated == 0)
			{
				int status;
				int wpid = waitpid(proc->pid, &status, WNOHANG);

				//Executa o processo por um determinado tempo.
				printf("Executando processo %d, de nome: %s e pid: %d\n", index, proc->name, proc->pid);
				kill(proc->pid, SIGCONT);	

				//Timeslice
				sleep(TIMESLICE);

				//Interrompe o processo
				printf("Interrompendo processo %d, de nome: %s e pid: %d\n", index, proc->name, proc->pid);
				kill(proc->pid, SIGSTOP);

				//Verifica se o processo atual, ja teve sua
				//execucao finalizada.
				if(wpid && !(proc->terminated) && WIFEXITED(status) && (WEXITSTATUS(status) == 0))
				{
					printf("Processo %d, de nome: %s e pid: %d\n finalizado com sucesso!", index, proc->name, proc->pid);

					//Salva a informacao de que o processo ja terminou.
					proc->terminated = 1;

					//Elimina o processo
					kill(proc->pid, SIGKILL);

					//Incrementa o contador de processos finalizados.
					doneProcesses += 1;
				}
			}	
		}
		//Incrementa o controlador de processos
		//Proximos processos
		i++;   
	}
}


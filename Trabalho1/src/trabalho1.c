#include "trabalho1.h"

/**
 * Processo
 */
struct process
{
 	int 	pid;	/* Identificacao do processo*/
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
int nProcesses;

/* Numero de processos finalizados*/
int doneProcesses = 0;

/***********************************/
/** Fim das variaveis de controle **/
/***********************************/


int main(int argc, char* argv[])
{
	int dispatcherType;

	//	Verifica os parametros recebidos
    if( argc < 3 )
    {
		printf("Error parameters\n");
		return -1;
	}

	dispatcherType = atoi(argv[1]);

	//	Verifica o tipo de escalonador
    if(dispatcherType < 1 || dispatcherType > 2)
    {
    	printf("Escalonamento indefinido\n");
    	return -1;
    }


	createProcessVector(dispatcherType, argv[2]);
    debugProcessVector();
    redirectOutput();

    switch(dispatcherType)
    {
    	case PRIORITY_DISPATCHER:
    		executePriority();
    		break;

    	case ROUND_ROBIN_DISPATCHER:
    		executeRoundRobin();
    		break;

    	default:
    		printf("Inconsistencia! \n");
    		return -1;
    }

	return 0;
}

/***********************************/
/**** Cria vetor de processos ******/
/***********************************/
void createProcessVector(int dispatcherType, char* inputFile)
{
    int i = 0;
	char execCommand[4];
	char program[50];
	char priorityLabel[11];
    int priorityValue;

    FILE *input = fopen(inputFile, "r");

	if(input == NULL)
	{
		printf("Error opening file");
		exit(1);
	}

    //	Aloca endereco de memoria para struct
    processes = (Process **) malloc(sizeof(Process *));

    while(fscanf(input, "%s %s ", execCommand, program) == 2)
    {
        if(dispatcherType == PRIORITY_DISPATCHER) //le prioridade
        {
            fscanf(input, "%s %d", priorityLabel, &priorityValue);
        }
        else //nao tem prioridade
        {
            priorityValue = 0;
        }

        //Aloca struct atual
        Process* currentProcess = (Process*) malloc(sizeof(Process));

        //atribui valores
        currentProcess->pid = 0;
        currentProcess->name = (char*) malloc(sizeof(char) * strlen(program));
        strcpy(currentProcess->name, program);
        currentProcess->param = priorityValue;
        currentProcess->done = 0;

        //Aloca mais um espaco no array
        processes = (Process **) realloc(processes, sizeof(Process *)*(i+1));

        //E aponta
        processes[i] = currentProcess;

        i++;
    }

    fclose(input);
    nProcesses = i;
}


/***********************************/
/****** Redirecionar Saida  ********/
/***********************************/
void redirectOutput()
{
    int fd2, output;

    if((fd2 = open("output.txt", O_RDWR|O_CREAT, O_TRUNC, 0666)) == -1)
    {
        perror("Error open()");
		return;
    }

    if((output = dup2(fd2,1)) == -1) //duplica stdout
    {
        perror("Error open()");
		return;
    }
}

/***********************************/
/**** Escalonador Round-Robin ******/
/***********************************/
void executeRoundRobin()
{
	int i = 0;
	int index;
	Process * proc;

	//	Executa ate que todos os processos tenham terminado
	while(doneProcesses < nProcesses)
	{
		//	Como o i 'e incrementado ate que todos os
		//	processos tenham sido executados por completo,
		//	ele pode assumir um valor que tende a infinito.
		//	Entao devemos pegar apenas um index dentro do dominio
		//	(de 0 ate nProcessess).
		index = i % nProcesses;

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
				printf("<< Interrompendo processo de nome: %s e pid: %d por tempo\n\n", proc->name, proc->pid);
				kill(proc->pid, SIGSTOP);

			}
			else
			{
				//	Processo filho executa o programa em
				//	questao sem parametros
				printf(">> Executando processo de nome: %s e pid: %d\n\n", proc->name, proc->pid);
				execve(proc->name, NULL, NULL);
			}
		//	No caso de ja ter sido inicializado
		}
		else
		{
			//	Verifica se o processo em questao
			//	ja teve sua execucao finalizada.
			//	Se ainda nao terminou:
			if(proc->done == 0)
			{
				int status;

				//	Passa o endereco da variavel status como
				//	destino da informacao referente ao status
				//	do processo;
				int wpid = waitpid(proc->pid, &status, WNOHANG);

				//Executa o processo por um determinado tempo.
				printf(">> Executando processo de nome: %s e pid: %d\n\n", proc->name, proc->pid);
				kill(proc->pid, SIGCONT);

				//Timeslice
				sleep(TIMESLICE);

				//Interrompe o processo
				printf("<< Interrompendo processo de nome: %s e pid: %d\n\n", proc->name, proc->pid);
				kill(proc->pid, SIGSTOP);

				//Verifica se o processo atual, ja teve sua
				//execucao finalizada.
				if(wpid && !(proc->done) && WIFEXITED(status) && (WEXITSTATUS(status) == 0))
				{
					printf("-- Processo de nome: %s e pid: %d finalizado com sucesso! \n\n", proc->name, proc->pid);

					//Salva a informacao de que o processo ja terminou.
					proc->done = 1;

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
        debugDoneProcesses();
        debugReadyProcesses();
	}
}

/***********************************/
/**** Escalonador por Prioridade ***/
/***********************************/
void executePriority()
{
	int i = 0;

	Process * proc;

//	Executa at'e que todos os processos tenham terminado
	while(doneProcesses < nProcesses)
	{
		//Pega o proximo processo da lista por prioridade
		proc = pickProcessByPriority(proc);

		//	Verifica se o processo ja foi inicializado
		//	Caso nao tenha sido , inicia a execucao.
		if(proc->pid == 0)
		{
			proc->pid = fork();

			//	Processo pai
			if(proc->pid != 0)
			{
				//Timeslice
				sleep(TIMESLICE);

				//	Interrompe o processo
				printf("<< Interrompendo processo de nome: %s e pid: %d por prioridade\n\n", proc->name, proc->pid);
				kill(proc->pid, SIGSTOP);

				//	Apos ter sido executado uma vez, o processo
				// 	tem sua prioridade reduziada (aumentada).
				if(proc->param < PRIOMIN)
				{
					proc->param = proc->param + 1;
				}
			}
			else
			{
				//	Processo filho executa o programa em
				//	questao sem parametros no caso.
				printf(">> Executando processo de nome: %s , pid: %d\n\n", proc->name, proc->pid);
				execve(proc->name, NULL, NULL);
			}
		}
		else
		{
			//	Verifica se o processo ja terminou.
			//	Caso nao tenha ter terminado ainda:
			if(proc->done == 0)
			{
				int status;

				//	Passa o endereco da variavel status como
				//	destino da informacao referente ao status
				//	do processo;
				int wpid = waitpid(proc->pid, &status, WNOHANG);

				//	Retoma a execucao do processo
				printf(">> Executando processo de nome: %s e pid: %d\n\n", proc->name, proc->pid);
				kill(proc->pid, SIGCONT);

				//	Timeslice
				sleep(TIMESLICE);

				//	Interrompe a execucao do programa.
				printf("<< Interrompendo processo de nome: %s e pid: %d por prioridade\n\n", proc->name, proc->pid);
				kill(proc->pid, SIGSTOP);

				//	Reduz a prioridade do processo apos
				//	ser executado mais uma vez.
				if(proc->param < PRIOMIN)
				{
					proc->param = proc->param + 1;
				}

				//Verifica se o processo atual, ja teve sua
				//execucao finalizada.
				if(wpid && !(proc->done) && WIFEXITED(status) && (WEXITSTATUS(status) == 0))
				{
					//Salva a informacao de que o processo ja terminou.
					proc->done = 1;

					//	Elimina o processo
					printf("-- Processo de nome: %s e pid: %d Finalizado com sucesso! \n\n", proc->name, proc->pid);
					kill(proc->pid, SIGKILL);

					//	Incrementa o numero de processos finalizados.
					doneProcesses++;
				}
			}
		}
		//Incrementa o contador
		i++;
        debugDoneProcesses();
        debugReadyProcesses();
	}
}


/***********************************/
/**** Escalonador por Prioridade ***/
/***********************************/
Process* pickProcessByPriority(Process* lastProcess)
{
	int i;

	//	Inicializa com o primeiro processo
	Process* temp = processes[0];

	for(i=0; i < nProcesses; i++)
	{
		//	Se o processo em questao ja esta finalizado
		//	troca o processo.
		if(temp->done)
		{
			temp = processes[i];
		}

		//	Verifica se o processo em questao tem prioridade
		//	menor (maior) do que o processo temporario (mais
		//	provavel a ser retornado).
		if(processes[i]->param <= temp->param)
		{
			//	Verifica se o ultimo processo 'e' nulo
			//	ou o processo temporario (mais provavel
			//	a ser executado) foi o ultimo a ser executado.
			//	Caso for, troca para o processo em questao.
			if((lastProcess == NULL) || (strcmp(temp->name, lastProcess->name) == 0))
			{
				temp = processes[i];
			}
		}
	}

	return temp;
}


void debugProcessVector()
{
    int i;

    printf("Processos encontrados:\n");

    for(i=0; i<nProcesses; i++)
    {
        printf("\nProcesso %d: pid - %d, name - %s, param - %d, done - %d\n", i+1,
        processes[i]->pid, processes[i]->name, processes[i]->param, processes[i]->done);
    }
}

void debugDoneProcesses()
{
    int i;
    if(doneProcesses != 0)
    {
        printf("|| Processos finalizados:\n");
        for(i=0; i<nProcesses; i++)
        {
            if(processes[i]->done == 1)
            {
                printf("|| Processo de nome: %s e pid: %d \n", processes[i]->name, processes[i]->pid);
            }
        }
    }
    else
    {
        printf("|| Ainda nao existem processos finalizados\n");
    }
}

void debugReadyProcesses()
{
    int i;
    if(doneProcesses != nProcesses)
    {
        printf("|| Processos prontos para execucao:\n");
        for(i=0; i<nProcesses; i++)
        {
            if(processes[i]->done == 0 && processes[i]->pid != 0)
            {
                printf("|| Processo de nome: %s e pid: %d \n", processes[i]->name, processes[i]->pid);
            }
        }
    }
    else
    {
        printf("|| Todos os processos foram finalizados\n\n");
    }
    printf("\n");
}

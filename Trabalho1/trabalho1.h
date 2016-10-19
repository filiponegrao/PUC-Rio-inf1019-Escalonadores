#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>

#define TIMESLICE 1
#define PRIOMIN 7
#define PRIOMAX 1
#define PRIORITY_DISPATCHER 1
#define ROUND_ROBIN_DISPATCHER 2

typedef struct process Process;

/* Funcoes auxiliares */
void createProcessVector(int dispatcherType, char* inputFile);
Process* pickProcessByPriority(Process* lastProcess);
static void debugProcessVector();

/* Dispatchers */
void executeRoundRobin();
void executePriority();


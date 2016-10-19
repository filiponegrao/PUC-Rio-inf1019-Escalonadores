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

typedef struct process Process;

void executeRoundRobin();
Process* pickProcessByPriority(Process* lastProcess);
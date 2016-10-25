#include <stdlib.h>

void handler(int sig, siginfo_t* info, void* vp);

void send(int signum, InfoHandler* handler);

int main (void) 
{
	
}



void handler(int sig, siginfo_t* info, void* vp)
{ 
  printf("%d\n", info->si_pid);
} 

void send(int signum, InfoHandler* handler)
{
  struct sigaction action, old_action;

  action.si_pid = getpid() 

  sigaction(signum, &action, NULL);
}
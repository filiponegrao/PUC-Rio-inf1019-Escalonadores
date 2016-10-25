#include <stdio.h>
#include <stdlib.h>

int main (void)
{
	char nome [100];

	sleep(5);
	printf("Digite seu nome \n");
	scanf("%s", nome);
	printf("Ola %s\n", nome);

	return 0;
}
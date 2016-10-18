#include <stdio.h>
#include <stdlib.h>


/* Estrutura de teste */
typedef struct teste
{
	int id;
} Teste;

Teste ** array;

void printArray();

int main(int argc, char const *argv[])
{
	int i;

	//Cria uma struct temporaria
	Teste * temp;

	//Aloca 1 ENDERECO de memoria para uma struct.
	//Em outras palavras aloca um espa'co inteiro,
	//pois 'e um endere'co apenas
	array = (Teste **) malloc(sizeof(Teste *));

	for (i = 0; i < 10; ++i)
	{
		printf("Aloca %d vez\n", i);

		//Aloca a struct da vez
		temp = (Teste *)malloc(sizeof(Teste));
		temp->id = i;

		//Aloca mais um espa'co de PONTEIRO no array
		array = (Teste **) realloc(array, sizeof(Teste *) * (i+1));

		//E aponta
		array[i] = temp;
	}

	printArray(array);

	return 0;
}

//AQUI TA ERRADO O CONT, TEM QUE DDAR UMA OLHADA MELHOR NISSO
// Mas acredito que salvando o numero em uma variavel
// global seja melhor do que dar o sizeof()
void printArray(Teste ** array)
{
	int i;

	int totalBytes;

	int cont;

	totalBytes = sizeof(array);

	printf("%d bytes\n", totalBytes);
	printf("%d bytes de tamanho da struct Teste \n", sizeof(Teste));

	cont = totalBytes/sizeof(Teste);

	for (i = 0; i < cont; ++i)
	{
		printf("Processo %d\n", array[i]->id);
	}
}
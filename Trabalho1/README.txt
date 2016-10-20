/*************************************************/
/**	- Escalonadores (Prioridade e Circular)	**/
/**	- Interpretador de comandos		**/
/**						**/
/**	INF1019 - Sistemas de Computacao    	**/
/**						**/
/*************************************************/

Autores:
	Filipo Negrao - 	1221846
	Fernanda Carvalho - 1411287


INSTRUCOES:

Editando as entradas:

	1 - ESCALONAMENTO POR PRIORIDADE:

		Para executar o escalonamento por prioridade
	o usuario devera editar o arquivo "entradaPrioridade.txt"
	e formata-lo no padrao: exec <programa> prioridade= <valor prioridade>
	sendo o “valor prioridade” algum número de 1 a 7 onde 1 'e a maior
	prioridade e 7 a menor.


	2 - ESCALONAMENTO CIRCULAR (ROUND-ROBIN):

		Para executar o escalonamento round-robin
	o usuario devera editar o arquivo "entradaRoundRobin.txt"
	no seguinte formato: exec <programa>


Para recompilar os programas:

  Se alguma mudanca for efetuada nos codigos fontes
e for necessario compilar o programa novamente, abra seu
terminal e execute o comando: bash build.sh. O arquivo
build.sh esta presente na mesma pasta

Rodando o escalonador:

	Para executar o escalonador de processos,
abra seu terminal, va ate a pasta onde se encontram
os arquivos e digite
./<nome arquivo executável> <tipo do escalonamento desejado> <entrada.txt>

* O nome do executável é trab1 pois já está definido em build.sh

** Os tipos de escalonamento estão definidos como:
1 - Prioridade
2 - Round Robin

*** As respectivas entradas são:
- entradaPrioridade.txt
- entradaRoundRobin.txt




Mais informacoes acesse: www.mynechat.com

/**********************************************/

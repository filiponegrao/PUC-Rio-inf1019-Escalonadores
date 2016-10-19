#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

	int i = 0;

	while(i < 8)
	{
		sleep(1);
		i++;
	}

	return 0;
}

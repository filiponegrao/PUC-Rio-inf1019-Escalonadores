#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{

	int i = 0;

	for (int i = 0; i < 10; ++i)
	{
		sleep(1);
	}

	return 0;
}

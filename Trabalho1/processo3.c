#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int i = 0;
	while(i < 4)
    {
		i++;
		sleep(1);
	}
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int	main(int argc, char **argv)
{
	int	pid;
	int	x;

	x = 2;
	pid = fork();
	if (pid == -1)
	{
		return (1);
	}
	if (pid == 0)
		x++;
	sleep(2);
	printf("Value of x = %d", x);
	printf("Hello from process %d\n", getpid());
	if (pid != 0)
	{
		wait(NULL);
	}
	return (0);
}

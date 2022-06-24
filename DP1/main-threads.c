#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int	mails;
pthread_mutex_t mutex;

mails = 0;

void	*routine()
{
	pthread_mutex_lock(&mutex);
	for (int i = 0; i < 1000000; i++)
	{
		mails++;
	}
	sleep(1);
	pthread_mutex_unlock(&mutex);
}

int	main(int argc, char **argv)
{
	pthread_t	th[4];

	pthread_mutex_init(&mutex, NULL);
	for( int i = 0; i< 4;i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL))
		{
			perror("Failed to create thread!\n");
			return (1);
		}
		printf("Thread %d has started!\n", i);
	}
	for (int i = 0; i < 4; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			return (2);
		}
		printf("Thread %d has finished execution!\n", i);
	}

	pthread_mutex_destroy(&mutex);
	printf("Number of mails = %d\n", mails);
	return (0);
}

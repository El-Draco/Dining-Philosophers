/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 08:49:20 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/21 15:56:03 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

#define THREAD_NO 4

pthread_mutex_t mutex_fuel;
pthread_mutex_t mutex_water;
int fuel = 50;
// void *fuel_fill(void *arg)
// {
// 	for (int i = 0; i < 5; i++)
// 	{
// 		pthread_mutex_lock(&mutex_fuel);
// 		fuel += 15;
// 		printf("Filled fuel: %d!\n", fuel);
// 		pthread_mutex_unlock(&mutex_fuel);
// 		sleep(1);
// 	}
// }
// void *car(void *arg)
// {
// 	while (fuel < 40)
// 	{
// 		printf("No fuel, waiting for refill!\n");
// 		sleep(1);
// 	}
// 	if (fuel >= 40)
// 	{
// 		pthread_mutex_lock(&mutex_fuel);
// 		fuel -= 40;
// 		printf("Got fuel. Now left: %d\n", fuel);
// 		pthread_mutex_unlock(&mutex_fuel);
// 	}
// 	else
// 		printf("No fuel!\n");
// }

void	*routine(void	*args)
{
	pthread_mutex_lock(&mutex_fuel);
	pthread_mutex_lock(&mutex_fuel);
	fuel+= 50;
	printf("Filled fuel to = %d\n", fuel);
	pthread_mutex_unlock(&mutex_fuel);
	pthread_mutex_unlock(&mutex_fuel);


}

int main(int argc, char **argv)
{
	pthread_t th[THREAD_NO];
	pthread_mutexattr_t recur_mutex_attrs;
	pthread_mutexattr_init(&recur_mutex_attrs);
	pthread_mutexattr_settype(&recur_mutex_attrs, PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&mutex_fuel, &recur_mutex_attrs);

	for (int i = 0; i < THREAD_NO; i++)
	{
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread!");
		}
		if (pthread_create(&th[i], NULL, &routine, NULL) != 0)
		{
			perror("Failed to create thread!");
		}
	}
	for (int i = 0; i < THREAD_NO; i++)
	{
		if (pthread_join(th[i], NULL) != 0)
		{
			perror("Failed to join thread!");
		}
	}
	printf("Fuel = %d\n", fuel);
	pthread_mutexattr_destroy(&recur_mutex_attrs);
	pthread_mutex_destroy(&mutex_fuel);

	return (0);
}

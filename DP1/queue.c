/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rriyas <rriyas@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 13:20:49 by rriyas            #+#    #+#             */
/*   Updated: 2022/06/20 15:34:53 by rriyas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

struct s_queue *newQueue(int size)
{
	struct s_queue *q;

	q = NULL;
	q = (struct s_queue *)malloc(sizeof(struct s_queue) * size);
	if (!q)
		return (q);
	q->philos = (struct s_philo *)malloc(sizeof(struct s_philo));
	if (!(q->philos))
		return (NULL);
	q->first = 0;
	q->last = 0;
	q->size = size;
	return (q);
}

int get_size(struct s_queue *q)
{
	if (!q)
		return (0);
	return (q->size);
}

int is_empty(struct s_queue *q)
{
	if (!q)
		return (0);
	return (0 == q->size);
}

struct s_philo *first(struct s_queue *q)
{
	if (!q || is_empty(q))
		return (NULL);
	return (q->first);
}

struct s_philo *last(struct s_queue *q)
{
	if (!q || is_empty(q))
		return (NULL);
	return (q->last);
}

void enqueue(struct s_queue *q, struct s_philo *p)
{
	if ( !p)
		return;
	printf("Inserting philospher to queue!\n");
	if (!q || q->size == 0)
	{
		q->first = p;
		q->last = p;
	}
	q->last->next = p;
	p->prev = q->last;
	q->last = p;
	q->first->prev = q->last;
	q->last->next = q->first;
	q->size++;
}

void dequeue(struct s_queue *q)
{
	if (!q)
		return;
	struct s_philo *tmp;
	tmp = q->first;
	q->first = q->first->next;
	q->first->prev = q->last;
	q->last->next = q->first;
	free(tmp);
}

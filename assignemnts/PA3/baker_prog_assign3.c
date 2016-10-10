/*----------------------------------------------------*
 * Author: Alex Baker                                 *
 * UNM ID: 101372834                                  *
 * UNM Email: alexebaker@unm.edu                      *
 *----------------------------------------------------*
 * CS 481                                             *
 * Programing Assignment 3                            *
 *----------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>

#include "buffer.h"
#include "my_semaphore.h"

void *producer(void *param);
void *consumer(void *param);
void init_buffer(buffer_item *buffer);
int get_sleep_time(void);

buffer_item buffer[BUFFER_SIZE];
int min_sleep = 1;
int max_sleep = 5;
int terminate = 0;
pthread_mutex_t buffer_mutex;
pthread_mutex_t terminate_mutex;
sem_t empty;
sem_t full;
int b_write;
int b_read;

int main(int argc, char *argv[])
{
    char *usage = "Usage: buffer <sleep time> <producer threads> <consumer threads>\n";
    int sleep_time = 0;
    int num_producers = 0;
    int num_consumers = 0;
    int i;

    // Check for the correct number of arguments
    if (argc != 4)
    {
        fprintf(stderr, "Wrong number of arguments!\n%s", usage);
        exit(EXIT_FAILURE);
    }
    else
    {
        sleep_time = (int) atoi(argv[1]);
        num_producers = (int) atoi(argv[2]);
        num_consumers = (int) atoi(argv[3]);
    }
    // initialize the buffer, mutex, and semaphores
    init_buffer(buffer);

    // create thread vars
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];
    pthread_attr_t attr;

    // create the producer and consumer threads
    for(i = 0; i < num_producers; i++)
    {
        pthread_attr_init(&attr);
        pthread_create(&producers[i], &attr, producer, NULL);
    }

    for(i = 0; i < num_consumers; i++)
    {
        pthread_attr_init(&attr);
        pthread_create(&consumers[i], &attr, consumer, NULL);
    }

    sleep(sleep_time);

    // the main thread is ready to terminate, set the flag for the children
    pthread_mutex_lock(&terminate_mutex);
    terminate = 1;
    pthread_mutex_unlock(&terminate_mutex);

    // wait for all of the children to terminate
    for (i = 0; i < num_producers; i++)
    {
        pthread_join(producers[i], NULL);
    }

    for (i = 0; i < num_consumers; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&buffer_mutex);
    pthread_mutex_destroy(&terminate_mutex);

    exit(EXIT_SUCCESS);
}

/* Initializes the buffer to 0's as well as the
 * mutex and semaphores */
void init_buffer(buffer_item *buffer)
{
    int i;
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        buffer[i] = 0;
    }
    pthread_mutex_init(&buffer_mutex, NULL);
    pthread_mutex_init(&terminate_mutex, NULL);
    empty = BUFFER_SIZE;
    full = 0;
    b_write = 0;
    b_read = 0;
    return;
}

/* inserts an item into the buffer */
int insert_item(buffer_item item)
{
    int i;
    int status = 0;

    // set the semaphore and get the lock on the buffer
    s_wait(&empty);
    status = pthread_mutex_lock(&buffer_mutex);

    // check if there were any errors
    if (status)
    {
        return -1;
    }

    // get the value of the semaphore and set the item in the buffer
    buffer[b_write] = item;
    b_write = (b_write + 1) % BUFFER_SIZE;

    // release the mutex and semaphore
    status = pthread_mutex_unlock(&buffer_mutex);
    s_signal(&full);

    if (status)
    {
        return -1;
    }
    return 0;
}

int remove_item(buffer_item *item)
{
    int i;
    int status = 0;

    // set the semaphore and lock the buffer
    s_wait(&full);
    status = pthread_mutex_lock(&buffer_mutex);

    // check for any errors
    if (status)
    {
        return -1;
    }

    // consume the next item in the buffer
    *item = buffer[b_read];
    b_read = (b_read + 1) % BUFFER_SIZE;

    // release the mutex and semaphore
    status = pthread_mutex_unlock(&buffer_mutex);
    s_signal(&empty);

    if (status)
    {
        return -1;
    }
    return 0;
}

void *producer(void *param)
{
    buffer_item item;

    while (true)
    {
        // check if the parent wants to terminate
        pthread_mutex_lock(&terminate_mutex);
        if (terminate)
        {
            // terminate this thread
            pthread_mutex_unlock(&terminate_mutex);
            pthread_exit(0);
        }
        pthread_mutex_unlock(&terminate_mutex);

        // sleep and create an item
        sleep(get_sleep_time());
        item = rand();
        if (insert_item(item))
        {
            fprintf(stderr, "Producer insert failed!\n");
        }
        else
        {
            fprintf(stdout, "Producer produced %d\n", item);
        }
    }
    pthread_exit(0);
}

void *consumer(void *param)
{
    buffer_item item;

    while (true)
    {
        // check is the thread should terminate
        pthread_mutex_lock(&terminate_mutex);
        if (terminate)
        {
            // terminate the thread
            pthread_mutex_unlock(&terminate_mutex);
            pthread_exit(0);
        }
        pthread_mutex_unlock(&terminate_mutex);

        // sleep and get an item from the buffer
        sleep(get_sleep_time());
        if (remove_item(&item))
        {
            fprintf(stderr, "Consumer remove failed!\n");
        }
        else
        {
            fprintf(stdout, "Consumer consumed %d\n", item);
        }
    }
    pthread_exit(0);
}

/* generates a random time to sleep */
int get_sleep_time(void)
{
    return min_sleep + rand() % max_sleep;
}

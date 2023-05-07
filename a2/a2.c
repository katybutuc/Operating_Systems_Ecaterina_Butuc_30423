#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>

sem_t semafor1, semafor2, semafor3;

void *thread_function(void *arg)
{
    int *t_nr = (int *)arg;
    if (*t_nr == 2)
    {
        sem_wait(&semafor1);//-1 la semafor1, nu scade sub 0 si asteapta pana poate scadea
        info(BEGIN, 9, 2);
        info(END, 9, 2);
        sem_post(&semafor2);//+1 la semafor2
    }
    if (*t_nr == 5)
    {
        info(BEGIN, 9, 5);
        sem_post(&semafor1);
        sem_wait(&semafor2);
        info(END, 9, 5);
    }
    if (*t_nr != 5 && *t_nr != 2)
    {
        info(BEGIN, 9, *t_nr);
        info(END, 9, *t_nr);
    }
    return NULL;
}

void *thread_function2(void *arg)
{
    int *t_nr = (int *)arg;
    sem_wait(&semafor3);
    info(BEGIN, 3, *t_nr);
    info(END, 3, *t_nr);
    sem_post(&semafor3);
    return NULL;
}

void *thread_function3(void *arg)
{
    int *t_nr = (int *)arg;
    info(BEGIN, 8, *t_nr);
    info(END, 8, *t_nr);

    return NULL;
}

int main()
{
    init();

    info(BEGIN, 1, 0);

    if (fork() != 0)
    {
        //fork face un proces copil
        if (fork() != 0)//returneaza 0 daca e copil
        {
            wait(NULL);//asteapta dupa copil sa termine
            wait(NULL);
            info(END, 1, 0);
        }
        else
        {
            info(BEGIN, 8, 0);
            pthread_t t[7];
            int *t_nr;
            int i;
            for (i = 1; i <= 6; i++)
            {
                t_nr = malloc(sizeof(int));
                *t_nr = i;
                pthread_create(&t[i], NULL, thread_function3, t_nr);
            }
            for (i = 1; i <= 6; i++)
                pthread_join(t[i], NULL);

            info(END, 8, 0);
        }
    }
    else
    {
        info(BEGIN, 2, 0);
        if (fork() != 0)
        {
            if (fork() != 0)
            {
                if (fork() != 0)
                {
                    if (fork() != 0)
                    {
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);
                        wait(NULL);
                        info(END, 2, 0);
                    }
                    else
                    {
                        info(BEGIN, 9, 0);
                        sem_init(&semafor1, 0, 0);//initializeaza semafor1 cu 0
                        sem_init(&semafor2, 0, 0);
                        pthread_t t[6];
                        int *t_nr;
                        int i;
                        for (i = 1; i <= 5; i++)
                        {
                            t_nr = malloc(sizeof(int));
                            *t_nr = i;
                            pthread_create(&t[i], NULL, thread_function, t_nr);
                        }
                        for (i = 1; i <= 5; i++)
                            pthread_join(t[i], NULL);//nu merge la urm linie de cod pana cand t[i] nu s-a terminat

                        info(END, 9, 0);
                    }
                }
                else
                {
                    info(BEGIN, 7, 0);
                    info(END, 7, 0);
                }
            }
            else
            {
                info(BEGIN, 4, 0);
                if (fork() != 0)
                {
                    wait(NULL);
                    info(END, 4, 0);
                }
                else
                {
                    info(BEGIN, 5, 0);
                    info(END, 5, 0);
                }
            }
        }
        else
        {
            info(BEGIN, 3, 0);
            if (fork() != 0)
            {
                pthread_t t[46];
                sem_init(&semafor3, 0, 4);//initializez semafor3 cu 4
                int *t_nr;
                int i;
                for (i = 1; i <= 45; i++)
                {
                    t_nr = malloc(sizeof(int));
                    *t_nr = i;
                    pthread_create(&t[i], NULL, thread_function2, t_nr);
                }
                for (i = 1; i <= 45; i++)
                    pthread_join(t[i], NULL);
                wait(NULL);
                info(END, 3, 0);
            }
            else
            {
                info(BEGIN, 6, 0);
                info(END, 6, 0);
            }
        }
    }

    return 0;
}
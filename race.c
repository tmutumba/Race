#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>


/*
** Compile and run this program, and make sure you get the 'aargh' error
** message. Fix it using a pthread mutex. The one command-line argument is
** the number of times to loop. Here are some suggested initial values, but
** you might have to tune them to your machine.
** Debian 8: 100000000
** Gouda: 10000000
** OS X: 100000
** You will need to compile your program with a "pthread" option.
*/

#define NUM_THREADS 2

int i;

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

void *foo (void *bar) {
    printf("in a foo thread, ID %ld\n", (long) pthread_self());

    assert(pthread_mutex_lock(&m) ==0);

    for (i = 0; i < *((int *) bar); i++) {
        int tmp = i;

        if (tmp != i) {
            printf ("aargh: %d != %d\n", tmp, i);
        }
    }

    assert(pthread_mutex_unlock(&m) ==0);
    pthread_exit ((void *)pthread_self());
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("You must supply a numerical argument.\n");
        exit(1);
    }

    int iterations = strtol(argv[1], NULL, 10);
    assert(errno == 0);
    assert(iterations > 0);

    pthread_t threads[NUM_THREADS];
    pthread_mutex_init(&m, NULL);

    for ( i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, foo, (void *) &iterations)) {
            perror ("pthread_create");
            return (1);
        }
    }

    for ( i = 0; i < NUM_THREADS; i++) {
        void *status;
        if (pthread_join (threads[i], &status)) {
            perror ("pthread_join");
            return (1);
        }
        printf("joined a foo thread, number %ld\n", (long) status);
    }

    assert(pthread_mutex_destroy(&m) ==0);
    return (0);
}
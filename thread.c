// thread0.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void *worker_thread(void *arg)
{
        printf("This is worker_thread()\n");
        //pthread_exit(NULL);
}

int main()
{
        /*pthread_t my_thread;
        int ret;

        printf("In main: creating thread\n");
        printf("%i\n", (int)&pthread_create);
        ret =  pthread_create(&my_thread, NULL, &worker_thread, NULL);
        printf("hello?\n");

        if(ret != 0) {
                printf("Error: pthread_create() failed\n");
                exit(EXIT_FAILURE);
        }

        pthread_exit(NULL);

        while (1) {

                printf("am i here?\n");
        }
        */

        int a = fork();


        if (a == 0) {
                printf("child a=%d\n", a);
                pthread_t my_thread;

                int ret =  pthread_create(&my_thread, NULL, &worker_thread, NULL);

                printf("ret = %d\n", ret);

        } else {
                printf("parent a=%d\n", a);

                int status;

                wait(&status);

                printf("status = 0x%X (%d)\n", status, status);
        }



        return 0;
}



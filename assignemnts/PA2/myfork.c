#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>


int fib(int x);


int main(int argc, char *argv[])
{
    pid_t pid, ppid;
    int numChildren = 2;
    int cpids[numChildren];
    int i = 0;
    clock_t start, end;
    double time;

    start = clock();
    ppid = getpid();
    for(i = 0; i < numChildren; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Fork Failed");
            exit(1);
        }
        else if (pid == 0)
        {
            printf("from C%d: own PID=%d, parent's PID=%d\n", i+1, getpid(), ppid);
            fib(20);
            exit(0);
        }
        else
        {
            cpids[i] = pid;
            wait(NULL);
        }
    }
    end = clock();

    printf("from P0: own PID=%d", ppid);
    for (i = 0; i < numChildren; i++)
    {
        printf(", PID of C%d=%d", i+1, cpids[i]);
    }
    time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000;
    printf(", total elapsed time in milliseconds=%.4f\n", time);
    return 0;
}


int fib(int x)
{
    int i = 0;
    int rint = rand() % 30;
    double dummy;

    for (i = 0; i < rint*100; i++)
    {
        dummy = (2.345 * i * 8.765) / 1.234;
    }

    if (x == 0)
    {
        return 0;
    }
    else if (x == 1)
    {
        return 1;
    }
    else
    {
        return fib(x-1) + fib(x-2);
    }
}

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>


int fib(int x);


int main(int argc, char *argv[])
{
    pid_t pid, ppid;
    int link[2];
    int numChildren = 2;
    int cpids[numChildren];
    int i = 0;
    clock_t start, end;
    double time;
    char buffer[1024];
    int readBytes;

    start = clock();
    ppid = getpid();
    for(i = 0; i < numChildren; i++)
    {
        if (pipe(link) == -1)
        {
            fprintf(stderr, "Pipe Failed.");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid < 0)
        {
            fprintf(stderr, "Fork Failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            printf("from C%d: own PID=%d, parent's PID=%d\n", i+1, getpid(), ppid);
            fib(20);

            dup2(link[1], 1);
            close(link[0]);
            close(link[1]);
            if (i == 0)
            {
                execl("/bin", "date", NULL);
            }
            else if (i == 1)
            {
                execl("/usr/bin", "who", NULL);
            }
            exit(EXIT_SUCCESS);
        }
        else
        {
            cpids[i] = pid;
            close(link[1]);
            while (read(link[0], buffer, sizeof(buffer)) != 0);
            printf("%s\n", buffer);
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

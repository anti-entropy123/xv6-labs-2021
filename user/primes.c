#include "kernel/types.h"
#include "user/user.h"

#define MAX_NUM 35
#define TRANS_SIZE 35 * 4
// #define MAX_LEN 255
// char buff[MAX_LEN];

int sieve_once(int *list)
{
    printf("prime %d\n", list[0]);
    int idx = 0;
    int base = list[0];
    for (int i = 1; list[i] != 0 && i < MAX_NUM; ++i)
    {
        if (list[i] % base == 0)
        {
            list[i] = 0;
        }
        else
        {
            list[idx++] = list[i];
        }
    }
    list[idx] = 0;
    return idx;
}

void show_list(int *list)
{
    for (int i = 0; i < MAX_NUM; ++i)
    {
        printf("list[%d]=%d\n", i, list[i]);
        if (list[i] == 0)
        {
            break;
        }
    }
}

void primes(int *list)
{
    int pipe_fd[2];
    pipe(pipe_fd);
    if (fork() != 0)
    {
        // parent
        // show_list(list);
        // int n =
        write(pipe_fd[1], list, TRANS_SIZE);
        // printf("proess[%d], write size=%d\n", getpid(), n);
        wait(0);
    }
    else
    {
        sleep(3);
        int list[MAX_NUM];
        // int n =
        read(pipe_fd[0], list, TRANS_SIZE);
        // printf("proess[%d], read size=%d\n", getpid(), n);

        close(pipe_fd[0]);
        close(pipe_fd[1]);
        // show_list(list);
        if (sieve_once(list) > 0)
        {
            primes(list);
        }
    }
}

int main(int argc, char *argv[])
{
    int list[MAX_NUM];
    for (int i = 0; i < MAX_NUM - 1; ++i)
    {
        list[i] = i + 2;
    }
    // show_list();
    primes(list);
    exit(0);
}
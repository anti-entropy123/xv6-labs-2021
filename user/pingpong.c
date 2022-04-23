#include "kernel/types.h"
#include "user/user.h"

struct pipe_pair
{
    int input;
    int output;
};

int read_pipe_pair(struct pipe_pair pair, char *buff, int len)
{
    return read(pair.output, buff, len);
}

int write_pipe_pair(struct pipe_pair pair, char *buff, int len)
{
    return write(pair.input, buff, len);
}

int close_pair(struct pipe_pair pair)
{
    close(pair.input);
    close(pair.output);
    return 0;
}

// void itoa(int i, char *buff, int size)
// {
//     int n = (i < 10 ? 2 : (i < 100 ? 3 : i < 1000 ? 4
//                                                   : 1)) -
//             1;

//     fprintf(1, "n=%d\n", n);
//     if (n >= size || n <= 0)
//     {
//         return;
//     }
//     buff[n--] = 0;
//     while (i > 0 && n >= 0)
//     {
//         fprintf(1, "n=%d, i=%d, buffer=%s\n", n, i, buff);
//         buff[n--] = i % 10 + '0';
//         i /= 10;
//     }
//     fprintf(1, "itoa result is %s", buff);
// }

int child_entrypoint(struct pipe_pair pair)
{
    char msg[3];
    read_pipe_pair(pair, msg, 1);
    int pid = getpid();
    fprintf(1, "%d: received ping\n", pid);
    write_pipe_pair(pair, "\0", 1); 
    close_pair(pair);
    return 0;
}

int parent_entrypint(struct pipe_pair pair)
{
    char buffer[3];
    // fprintf(1, "parent: write 1 byte");
    write_pipe_pair(pair, "\0", 1);
    read_pipe_pair(pair, buffer, 3);
    fprintf(1, "%d: received pong\n", getpid());
    close_pair(pair);
    return 0;
}

int main(int argc, char *argv[])
{
    int pipe_pair[2];
    struct pipe_pair parent_pair;
    struct pipe_pair child_pair;
    pipe(pipe_pair);
    parent_pair.output = pipe_pair[0];
    child_pair.input = pipe_pair[1];
    pipe(pipe_pair);
    parent_pair.input = pipe_pair[1];
    child_pair.output = pipe_pair[0];

    if (fork())
    {
        parent_entrypint(parent_pair);
    }
    else
    {
        child_entrypoint(child_pair);
    }

    exit(0);
}
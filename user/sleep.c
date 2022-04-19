#include "kernel/types.h"
#include "user/user.h"
// #include "user/ulib.c"

int user_sleep(int second)
{
    sleep(second);
    return 0;
}

int main(int argc, char *argv[])
{
    int second;
    char *s;
    if (argc <= 1)
    {
        fprintf(2, "sleep: need sleep time\n");
        exit(1);
    }
    s = argv[1];

    while (1)
    {
        // fprintf(2, "current is %c\n", *s);
        if (*s == '\0')
            break;
        if (!('0' <= *s && *s <= '9'))
        {
            fprintf(2, "sleep: wrong sleep time %s\n", argv[1]);
            exit(1);
        }
        s++;
    }
    second = atoi(argv[1]);
    fprintf(1, "you need sleep %d second", second);
    user_sleep(second);
    exit(0);
}
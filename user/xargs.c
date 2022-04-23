#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

char buf[512];

int base_args_cnt;
char *lines[512];
int sub_argc;
char *sub_argv[MAXARG];

int read_from_std()
{
    int buf_idx = 0;
    while (1)
    {
        char t[128];
        int len = read(0, t, sizeof(t));
        if (len == 0)
        {
            break;
        }
        // printf("read, t='%s', len=%d\n", t, len);
        // printf("read, t='%s', len=%d\n", t, len);
        strcpy(buf + buf_idx, t);
        buf_idx += len;
    }

    // fprintf(2, "read_from_std, buf='%s', len=%d\n", buf, buf_idx);
    buf[buf_idx] = '\0';
    return buf_idx;
}

int is_newline_char(char c)
{
    return c == '\n';
}

int is_space_char(char c)
{
    return c == ' ';
}

int split_lines()
{
    int line_idx = 0;
    if (!is_newline_char(buf[0]))
    {
        lines[line_idx++] = buf;
    }
    for (int i = 1; buf[i] != '\0'; ++i)
    {
        if (is_newline_char(buf[i]))
        {
            buf[i] = '\0';
        }
        else if (buf[i - 1] == '\0')
        {
            lines[line_idx++] = buf + i;
        }
    }
    return line_idx;
}

int split_words(char *line)
{
    int word_idx = 0;
    char **ad_sub_argv = sub_argv + base_args_cnt;
    if (!is_space_char(line[0]))
    {
        ad_sub_argv[word_idx++] = line;
    }
    for (int i = 1; line[i] != '\0'; ++i)
    {
        if (is_space_char(line[i]))
        {
            line[i] = '\0';
        }
        else if (line[i - 1] == '\0')
        {
            ad_sub_argv[word_idx++] = line + i;
            if (word_idx == sizeof(sub_argv) - base_args_cnt)
            {
                break;
            }
        }
    }
    ad_sub_argv[word_idx] = 0;
    return word_idx + base_args_cnt;
}

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(2, "need args\n");
    }
    base_args_cnt = argc - 1;
    for (int i = 0; i < base_args_cnt; ++i)
    {
        sub_argv[i] = argv[i + 1];
    }

    read_from_std();
    // printf("main: buff='%s'\n", buf);
    int line_cnt = split_lines();
    for (int i = 0; i < line_cnt; ++i)
    {
        // printf("lines[%d]=%s\n", i, lines[i]);
        // int word_cnt =
        split_words(lines[i]);
        // for (int j = 0; j < word_cnt; ++j)
        // {
        //     // printf("sub_argv[%d]=%s\n", j, sub_argv[j]);
        // }
        if (fork() == 0)
        {
            exec(argv[1], sub_argv);
        }
        else
        {
            wait(0);
        }
    }
    exit(0);
}
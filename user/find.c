#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

int is_dir(const char *path)
{
    struct stat st;
    if (stat(path, &st) < 0)
    {
        return 0;
    }
    return st.type == T_DIR;
}

void path_join(const char *base, const char *sub_path, char *buf)
{
    // fprintf(2, "need join %s to base %s, store in buf=%s\n", sub_path, base, buf);
    memcpy(buf, base, strlen(base));
    char *p = buf + strlen(base);
    if (*(p - 1) != '/')
        *(p++) = '/';
    int i;
    for (i = 0; i < strlen(sub_path); ++i)
    {
        *(p + i) = sub_path[i];
    }
    *(p + i) = 0;
    // fprintf(2, "join result %s\n", buf);
}

void find(const char *dir, char *target)
{
    // fprintf(2, "find target=%s, in dir=%s\n", target, dir);
    int fd = open(dir, 0);
    if (fd < 1)
    {
        fprintf(2, "can't open %s\n", dir);
        exit(1);
    }
    struct dirent entry;
    while (read(fd, &entry, sizeof(entry)) == sizeof(entry))
    {
        // fprintf(2, "entry.name=%s, strcmp(entry.name, '.')=%d, strcmp(entry.name, '..')=%d\n", entry.name, strcmp(entry.name, "."), strcmp(entry.name, ".."));
        if (entry.inum == 0 || strcmp(entry.name, ".") == 0 || strcmp(entry.name, "..") == 0)
        {
            continue;
        }

        char item_path[512];
        path_join(dir, entry.name, item_path);

        if (strcmp(entry.name, target) == 0)
        {
            printf("%s\n", item_path);
        }
        if (is_dir(item_path))
        {
            // fprintf(2, "item %s is a directory\n", item_path);
            find(item_path, target);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        fprintf(2, "need a specific path and a target name\n");
        exit(1);
    }
    char *path = argv[1];
    if (!is_dir(path))
    {
        fprintf(2, "arg %s is not a directory", path);
        exit(1);
    }
    find(path, argv[2]);
    exit(0);
}
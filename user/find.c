// find all the files in a directory tree with a specific name

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *target) {
    // path: current dir path being searched
    // target: target file name

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    // open the directory
    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // get file/directory info
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    switch(st.type) {
    case T_FILE:
        // for files, check if name matches
        p = path + strlen(path);
        while (p > path && *p != '/')
            p--;
        p++;
        if (strcmp(p, target) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR:
        // for directories, search through its contents
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        
        // read directory entries
        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
                
            // skip "." and ".."
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                continue;
                
            // copy the name into the path buffer
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            
            // recursively search this entry
            find(buf, target);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "Usage: find <directory> <filename>\n");
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
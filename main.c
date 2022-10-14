#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>


long path_max;
size_t _MAX_PATH_SIZE, size;

static inline void *mallocOrDie(size_t MemSize)
{
    void *AllocMem = malloc(MemSize);
    /* Some implementations return null on a 0 length alloc,
     * we may as well allow this as it increases compatibility
     * with very few side effects */
    if(!AllocMem && MemSize)
    {
        //TODO handle error
        fprintf(stderr, "Cannot allocate memory\n");
        exit(-1);
    }
    return AllocMem;
}

void printdir(char *dir, char* depth)
{
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    size_t count_of_block_depth;
    size_t count_of_block_entry;
    if ((dp=opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while ((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* found a directory, but ignore . and .. */
            if (strcmp(".", entry->d_name) == 0 ||
                strcmp("..", entry->d_name) == 0)
                continue;
            /* recurse at a new indent level */
            count_of_block_depth = strlen(depth) / _MAX_PATH_SIZE + 1;
            count_of_block_entry = strlen(entry->d_name) / _MAX_PATH_SIZE + 1;
            char *tempdepth = mallocOrDie((sizeof(char)*_MAX_PATH_SIZE) * (count_of_block_depth+count_of_block_entry) + 1);
            strcpy(tempdepth, depth);
            if (strcmp(tempdepth, "/") != 0)
                strcat(tempdepth, "/");
            strcat(tempdepth, entry->d_name);
            printdir(entry->d_name, tempdepth);
            free(tempdepth);
        }
        else{
            printf("%s/%s\n", depth, entry->d_name);
        }
    }
    chdir("..");
    closedir(dp);
}

int main(int argc, char** argv) {
    char *root_dir;
    size_t count_of_block_root_dir = 0;
    path_max = pathconf(".", _PC_PATH_MAX);
    if (path_max == -1)
        _MAX_PATH_SIZE = 1024;
    else if (path_max > 10240)
        _MAX_PATH_SIZE = 10240;
    else
        _MAX_PATH_SIZE = path_max;
    if (argc<2) {fprintf(stderr, "Wrong count of element. Required>=1 : Received: %d\n", argc-1); exit(-1);}

    // check if end with "/"
    // check strlen != 0 in case "/"
    count_of_block_root_dir = strlen(argv[1]) / _MAX_PATH_SIZE + 1;
    root_dir = mallocOrDie((sizeof(char)*_MAX_PATH_SIZE*7) * count_of_block_root_dir + 1);
    if ((strlen(argv[1])-1)!=0 && argv[1][strlen(argv[1])-1] == '/') { // ends with "/"
        strncpy(root_dir, argv[1], strlen(argv[1])-1);
    }
    else {
        strcpy(root_dir, argv[1]);
    }
    printdir(root_dir, root_dir);
    free(root_dir);
    exit(0);
}
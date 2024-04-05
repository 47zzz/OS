#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[])
{
    char buffer[BUF_SIZE];
    int fd[2];
    pid_t pid;
    ssize_t bytes_read;

    if(pipe(fd) == -1)
    {
        fprintf(stderr, "pipe failed");
        return 1;
    }
    pid = fork();

    if(pid < 0)
    {
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0)
    {
        close(fd[1]);
        FILE *dest_file = fopen(argv[2], "w");
        
        while ((bytes_read = read(fd[0], buffer, BUF_SIZE)) > 0) {
            if (fwrite(buffer, 1, bytes_read, dest_file) != bytes_read) {
                printf("child error\n");
                return 0;
            }
        }
        close(fd[0]);
        fclose(dest_file);
    }
    else
    {
        close(fd[0]);
        FILE *src_file = fopen(argv[1], "r");
        if (src_file == NULL) {
            fprintf(stderr, "read file failed");
            return 1;
        }
        while ((bytes_read = fread(buffer, 1, BUF_SIZE, src_file)) > 0) {
            ssize_t bytes_written = write(fd[1], buffer, bytes_read);
            if (bytes_written != bytes_read) {
                if (bytes_written == -1) {
                    printf("parent error\n");
                } else {
                    printf( "Error: Could not write all bytes to pipe.\n");
                }
                return 0;
            }
        }

        close(fd[1]);
        fclose(src_file);
        wait(NULL);
    }
    return 0;
}
       
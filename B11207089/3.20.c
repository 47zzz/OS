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
    ssize_t bytes_read, bytes_written;

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
        int txtfile = open(argv[2], O_CREAT|O_RDWR);
        while ((bytes_read = read(fd[0], buffer, BUF_SIZE)) > 0) 
        {
            bytes_written = write(txtfile, buffer, bytes_read);
            if (bytes_written == -1) 
            {
                printf("writed\n");
                close(fd[0]);
                close(txtfile);
            }
        }
    }
    else
    {
        close(fd[0]);
        int txtfile = open(argv[1], O_RDONLY);
        if(txtfile == -1)
        {
            fprintf(stderr, "read file failed");
            return 1;
        }
        while ((bytes_read = read(txtfile, buffer, BUF_SIZE)) > 0) 
        {
            bytes_written = write(fd[1], buffer, bytes_read);
            if (bytes_written == -1) 
            {
                printf("writing\n");
                close(fd[1]);
                close(txtfile);
            }
        }
    }
    return 0;
}
       
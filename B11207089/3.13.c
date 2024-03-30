#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/type.h>
#include <unistd.h>
#include <sys/wait.h>

int fibonacci(int n){
    if(n == 0){
        
        return 0;
    }
    if(n == 1){
        
        return 1;
    }
    
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main()
{
    pid_t pid;
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0){
        int n = 0;
        scanf("%d", &n);
        for(int i = 0; i < n; i++)
        {
            printf("%d ",fibonnacci(i));
        }
    }
    else{
        wait(NULL);
        printf("Child Complete\n");
    }
}

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

#define MAX_SEQUENCE 10

typedef struct{
    long fib_seq[MAX_SEQUENCE];
    int  seq_size;
}shared_data;

int fibonacci(int n){
    if(n == 0){
        
        return 0;
    }
    if(n == 1){
        
        return 1;
    }
    
    return fibonacci(n - 1) + fibonacci(n - 2);
}


int main(int argc, char *argv[])
{
    int shm_fd;
    shared_data *ptr;
    const char *name = "HW2";
    
    shm_fd = shm_open(name, O_CREAT|O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(shared_data));
    ptr = (shared_data*)mmap(0, sizeof(shared_data), PROT_WRITE, MAP_SHARED, shm_fd, 0);

    int n = atoi(argv[1]);
 
    ptr->seq_size = n;
    if(n > 10 || n < 0)
        {
            printf("Please entry a number less than 10\n");
            return 0;
        }


    pid_t pid;
    pid = fork();
    
    if(pid < 0){
        fprintf(stderr, "Fork Failed\n");
        return 1;
    }
    else if (pid == 0){
        
        for(int i = 0; i < ptr->seq_size; i++)
        {
            ptr->fib_seq[i] = fibonacci(i);
        }
    
    }
    else{
        
        wait(NULL);
        

        for(int i = 0; i < ptr->seq_size; i++)
        {
            printf("%ld ",ptr->fib_seq[i]);
        }
    
        shm_unlink(name);
    }
return 0;
}

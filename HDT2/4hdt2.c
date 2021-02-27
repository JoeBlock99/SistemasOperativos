#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    pid_t fork1;
    fork1 = fork();
    int loop = 1;
    int i = 0;
    if(fork1==0){
        for(i;i<4000000;i++){
            printf("%d\n",i);
        }
    }else{
        while(loop != 0){

        }
    }
    return(0);
}

#include<stdio.h>
#include<unistd.h>

int main(){
    fork();
    fork();
    fork();
    fork();
    printf("forks\n");
    return(0);
}
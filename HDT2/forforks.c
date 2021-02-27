#include<stdio.h>
#include<unistd.h>

int main(){
    int i = 0;
    for(i;i<4;++i){
        fork();
    }
    printf("forks\n");
    return 0;
}
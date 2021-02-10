#include<stdio.h>
#include<unistd.h>

int main(){
	int f = fork();
	if(f==0){
		execl("Ejercicio_1",(char*)NULL);
	}else{
		printf("%d\n",(int)getpid());
		execl("Ejercicio_1",(char*)NULL);
	}
	return(0);
}
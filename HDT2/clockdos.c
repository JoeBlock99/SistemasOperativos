#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
	pid_t funo, fdos, ftres;
	clock_t start, stop;
	int i;
	
	start = clock();
	funo = fork();
	if(funo==0){
		fdos = fork();
		if(fdos == 0){
			ftres = fork();
			for(i=0; i<1000000;i++){
				printf("%d\n", i);
			}
		}else{
			wait(NULL);
			for(i=0; i<1000000;i++){
				printf("%d\n", i);
			}
		}
		wait(NULL);
		for(i=0; i<1000000;i++){
			printf("%d\n", i);
		}
		
	}else{
		wait(NULL);
		stop = clock();
		double total = stop - start;
		printf("%f\n",total);
	}
	
}

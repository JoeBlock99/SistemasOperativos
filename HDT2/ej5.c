#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<sys/shm.h>

int main(){
	pid_t pid = fork();
	if(pid == 0){
		int response = execl("ipc","ipc", "5", "A", "a", (char *)NULL);

	}else if(pid > 0){
		wait(NULL);
		int response = execl("ipc","ipc", "6", "B", "b", (char *)NULL);
	}
}

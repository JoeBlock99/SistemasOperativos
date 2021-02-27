#include<stdio.h>
#include<time.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<string.h>
#include<stdlib.h>
#include<fcntl.h>
#include<errno.h>

#define WRITE 1
#define READ 0
#define BUFF 100

int main(int argc, char* argv[]){
	clock_t start, stop;
	int n = atoi(argv[1]);
	char* x = argv[2];
	char* inst = argv[3];
	const int largo = 64;
	const char *nombre = "ej5";
	char mensaje[BUFF];
	int filedescriptor[2];
	
	start = clock();
	if(pipe(filedescriptor) == -1){
		fprintf(stderr, "ERROR");
		printf("DESC: %s\n", strerror(errno));
		return 1;
	}
	
	int shm;
	void *ptr;
	shm = shm_open(nombre, O_CREAT |O_EXCL | O_RDWR, 0666);
	if(shm > 0){
		printf("%s: Objeto de la memoria compartida: %d\n", inst, shm);
		ftruncate(shm, largo);
	}else{
		printf("%s: Objeto de memoria compartida CREADO %s %d\n", inst,inst,shm);
	}
	int file_descriptor = shm;
	ptr = mmap(0, largo, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
	
	printf("%s: Memoria compartida: %s %s %p\n", inst, nombre, inst, ptr);
	
	pid_t process_id = fork();
	if(process_id == 0){
		printf("%s: hijo: %d\n", inst, process_id);
		char* string;
		close(filedescriptor[WRITE]);
		
		read(filedescriptor[READ], mensaje, BUFF);
		sprintf(ptr, "%s", mensaje);
		ptr += strlen(mensaje);
		close(filedescriptor[READ]);
	}else if(process_id > 0){
		printf("%s: padre: %d\n", inst, process_id);
		close(filedescriptor[READ]);
		for(int i=1 ; i <= largo; i++){
			if(i%n == 0){
				write(filedescriptor[WRITE], x, strlen(x));
			}
		}
		close(filedescriptor[WRITE]);
		wait(NULL);
		printf("%s: La memoria compartida tiene: %s\n", inst, (char *)ptr);
		
		int borrar = shm_unlink(nombre);
		if(borrar == -1){
			printf("%s: ERROR %s\n", inst, nombre);
			printf("%s: DESC: %s\n", inst, strerror(errno));
			exit(-1);
		}
	}
	stop = clock();
	int total = stop - start;
	printf("Tiempo transcurrido: %d\n", total);
	
	return 0;
}


#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <omp.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <sys/wait.h>

int arr[9][9];
int fchecked, cchecked, cuachecked = 1;

void *filas(void *params){
	omp_set_nested(1);
	omp_set_num_threads(9);
	fchecked = 1;
	#pragma omp parallel for schedule(dynamic)
	for(int i = 0; i < 9; i++){
		printf("TID (filas): %ld\n", syscall(SYS_gettid));
		int check[] = {1,2,3,4,5,6,7,8,9};
		#pragma omp parallel for schedule(dynamic)
		for(int j = 0; j < 9; j++){
			if(arr[i][j] > 9 || check[arr[i][j]-1] != arr[i][j]){
				fchecked = 0;
			}
			check[arr[i][j]-1] = 0;
		}
	}
	pthread_exit(0);
}
void *columnas(void *params){
	omp_set_nested(1);
	omp_set_num_threads(9);
	cchecked = 1;
	#pragma omp parallel for 
	for(int i = 0; i < 9; i++){
		printf("TID (columnas): %ld\n", syscall(SYS_gettid));
		int check[] = {1,2,3,4,5,6,7,8,9};
		#pragma omp parallel for schedule(dynamic)
		for(int j = 0; j < 9; j++){
			if(arr[j][i] > 9 || check[arr[j][i]-1] != arr[j][i]){
				cchecked = 0;
			}
			check[arr[j][i]-1] = 0;
		}
	}
	pthread_exit(0);	
}
void *cuadros(void *params){
	omp_set_nested(1);
	omp_set_num_threads(9);
	int cuad[9] = {};
	int cntr = 0;	
	
	int locate[3] = {0, 3, 6};
	#pragma omp parallel for schedule(dynamic)
	for(int n = 0; n < 3; n++){
		printf("TID (subarr): %ld\n", syscall(SYS_gettid));
		cntr = 0;
		#pragma omp parallel for schedule(dynamic)
		for(int i = locate[n]; i < locate[n] + 3; i++){
			#pragma omp parallel for schedule(dynamic)
			for(int j = locate[n]; j < locate[n] + 3; j++){
				int x = arr[i][j] - 1;
				cuad[x] = arr[i][j];
				cntr += arr[i][j];
			}
		}
		if(cntr != 45 || sizeof(cuad)/sizeof(int) != 9){
			cuachecked = 0;
		}
	}
	cuachecked = 1;
	pthread_exit(0);
}

void main() {
	omp_set_nested(1);
	omp_set_num_threads(1);
	char *fname = "sudoku";
	int fd = open(fname, O_RDONLY);
	struct stat fs;
	char *buf, *buf_end;
	char *begin, *end, c;
	char pid_str[10];
	pthread_t tid;
	pthread_attr_t attr;


	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			arr[i][j] = 0;
		}
	}

	fstat(fd, &fs);

	buf = mmap(0, fs.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (buf == (void*) -1) {
		err(1, "mmap: %s", fname);
		close(fd);
		return;
	}

	buf_end = buf + fs.st_size;
	begin = end = buf;
	while (1) {
		if (! (*end == '\r' || *end == '\n')) {
			if (++end < buf_end) continue;
		} else if (1 + end < buf_end) {
			c = *(1 + end);
			if ( (c == '\r' || c == '\n') && c != *end) ++end;
		}

		printf("%s\n", begin);

		int contX, contY = 0;
		for (int i = 0; i < strlen(begin); i++) {
			if (contY < 9) {
			    	arr[contX][contY] = begin[i]-48;
			    	contY++;
			}
			if (contY == 9) {
				contX++;
				contY = 0;
			}
		}
		if ((begin = ++end) >= buf_end) break;
	}

	munmap(buf, fs.st_size);
	close(fd);

	int pid = getpid();
	printf("%d\n", pid);
	pid_t fork0 = fork();
	
	if(fork0 == 0){
		sprintf(pid_str, "%d", pid);
		execl("/bin/ps", "ps", "-p", pid_str, "-lLf", NULL);
	}else{
		pthread_create(&tid, &attr, columnas, fname);
		pthread_join(tid, NULL);
		printf("%ld\n", syscall(SYS_gettid));
		wait(NULL);
		pthread_create(&tid, &attr, filas, fname);
		
		if(fchecked == 1 && cchecked == 1 && cuachecked == 1){
			printf("%s\n", "Sudoku valido");
		} else{
			printf("%s\n", "Sudoku invalido");
		}
		pid_t fork1 = fork();
		
		if(fork1 == 0){
			sprintf(pid_str, "%d", pid);
			execlp("/bin/ps", "ps", "-p", pid_str, "-lLf", NULL);
		} else{
			wait(NULL);
			return;
		}
	}
	return;
}
	
	
	


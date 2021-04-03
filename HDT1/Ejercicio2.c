#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]){
	char linea[1024];
	FILE *readfile;
	FILE *writefile;

	readfile = fopen(argv[1], "r");
	writefile = fopen(argv[2], "w");
	while(fgets(linea,1024, (FILE*) readfile)){
		fprintf(writefile, "%s", linea);
	}
	fclose(readfile);
	fclose(writefile);
}
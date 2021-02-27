#include<stdio.h>
#include<time.h>
int main(){
	clock_t start, stop;
	int x, y, z;
	start = clock();
	for(x=0;x<1000000;++x){printf("%d\n", x);}
	for(y=0;y<1000000;++y){printf("%d\n", y);}
	for(z=0;z<1000000;++z){printf("%d\n", z);}
	stop = clock();
	double total = stop-start;
	printf("%f\n",total);
	return 0;
}

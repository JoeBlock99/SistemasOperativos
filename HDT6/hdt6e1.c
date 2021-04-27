#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define N 10
static int res = 5;
static int tcicle = 5;
static int MAX_RAND = 10;

FILE *fop;

// definicion de semaforo para manejar la toma y devolucion de recursos
sem_t semaf;

// mutex y pcnd para gestionar el underflow
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pcnd = PTHREAD_COND_INITIALIZER;

void *func(void *arg) {
  fprintf(fop, "Thread %ld starts task\n" , (long)arg);
  for (int i = 0; i < tcicle; i++) {
    // Se consume un recurso
    sem_wait(&semaf);
    pthread_mutex_lock(&mutex);
    fprintf(fop,"Thread %d intentando usar algun recurso\t\t - recursos disponibles: %d\n", (long)arg, res);
    while (1 > res) {
      sem_post(&semaf);
      pthread_cond_wait(&pcnd, &mutex);
    }
    res--;
    int wt = rand() % MAX_RAND + 1;
    fprintf(fop,"Thread %ld usando recurso por %d S\t\t\t - recursos disponibles: %d\n", (long)arg, (int) wt, res);
    pthread_mutex_unlock(&mutex);
    sem_post(&semaf);

    sleep(wt);

    sem_wait(&semaf);
    pthread_mutex_lock(&mutex);
    res++;
    fprintf(fop, "Thread %ld ha terminado de usar el recurso\t\t\t - recursos disponibles: %d\n" , (long)arg, res);
    pthread_cond_signal(&pcnd);
    pthread_mutex_unlock(&mutex);
    sem_post(&semaf);
    // Se devuelve el recurso usado
  }
  pthread_exit(NULL);
  return NULL;
}

int main() {
  fop = fopen("outpute1.txt", "w+");
  // Se inicializan las variables de sincronizacion
  sem_init(&semaf, 0 , 1);
  pthread_cond_init(&pcnd, NULL);
  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL));

  // Creacion de la thread pool
  pthread_t thread[N];

  // Se crean los threads
  long n;
  for (n = 1; n <= N; n++) {
    int rt = pthread_create(&thread[n], NULL, &func, (void *)n);
    if (rt != 0) {
      printf("Pthread no pudo ser creado\n");
      exit(EXIT_FAILURE);
    }
  }

  // Se espera a los threads
  for(int i = 1; i<=N ; i++){
    pthread_join(thread[i], NULL);
  }

  // Se termina del programa
  pthread_mutex_destroy(&mutex);
  fclose(fop);
  pthread_exit(NULL);
}

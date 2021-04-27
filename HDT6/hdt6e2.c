#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

#define N 15

static int res = 5;
static int sizeCiclo = 10;
static int MAX_RAND = 4;

// se define un semaforo para manejar la toma y devolucion de recursos
sem_t semaf;

// mutex y pcond para gestionar el underflow
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t pcond = PTHREAD_COND_INITIALIZER;

FILE *fop;

int decrease_count(int count) {
  if (res < count) {
    return -1;
  } else {
    res -= count;
    return 0;
  }
}

int increase_count(int count) {
  res+=count;
  return 0;
}

void *func(void *arg) {
  fprintf(fop, "Thread %ld starts task\n" , (long)arg);
  for (int i = 0; i < sizeCiclo; i++) {
    // Se consume un recurso
    sem_wait(&semaf);
    pthread_mutex_lock(&mutex);
    int res_to_use = rand() % MAX_RAND + 1;
    fprintf(fop,"El thread %ld intenta usar los recursos %d\t\t - recursos disponibles: %d\n", (long)arg, res_to_use , res);
    while (res_to_use > res) {
      sem_post(&semaf);
      pthread_cond_wait(&pcond, &mutex);
    }
    decrease_count(res_to_use);
    int wt = rand() % MAX_RAND + 1;
    fprintf(fop,"El thread %ld usando recurso por %d S\t\t\t - recursos disponibles: %d\n", (long)arg, (int) wt, res);
    pthread_mutex_unlock(&mutex);
    sem_post(&semaf);

    sleep(wt);

    sem_wait(&semaf);
    pthread_mutex_lock(&mutex);
    increase_count(res_to_use);
    fprintf(fop, "Thread %ld ha terminado de usar el recurso %d\t\t\t - recursos disponibles: %d\n" , (long)arg, res_to_use, res);
    pthread_cond_signal(&pcond);
    pthread_mutex_unlock(&mutex);
    sem_post(&semaf);
    // Se retorna el recurso usado
  }
  pthread_exit(NULL);
  return NULL;
}

int main() {
  fop = fopen("output-2.txt", "w+");
  // se inicializalas variables de sincronizacion
  sem_init(&semaf, 0 , 1);
  pthread_cond_init(&pcond, NULL);
  pthread_mutex_init(&mutex, NULL);
  srand(time(NULL));

  // Se crea la thread pool
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

  // Se termina el programa
  pthread_mutex_destroy(&mutex);
  pthread_exit(NULL);
}

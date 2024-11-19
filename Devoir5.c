#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t* semaphores;
int n;


void* thread_function(void* arg) {
    int id = *((int*)arg);

    while (1) {

        sem_wait(&semaphores[id]);
        printf("Thread %d a fini\n", id + 1);


        sem_post(&semaphores[(id + 1) % n]);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nombre_de_threads>\n", argv[0]);
        return 1;
    }

    n = atoi(argv[1]);
    if (n <= 0) {
        fprintf(stderr, "Le nombre de threads doit être un entier positif.\n");
        return 1;
    }

    pthread_t threads[n];
    int thread_ids[n];
    semaphores = malloc(n * sizeof(sem_t));

    for (int i = 0; i < n; i++) {
        if (sem_init(&semaphores[i], 0, (i == 0) ? 1 : 0) == -1) {
            perror("Erreur d'initialisation des sémaphores");
            free(semaphores);
            return 1;
        }
        thread_ids[i] = i;
    }


    for (int i = 0; i < n; i++) {
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("Erreur de création du thread");
            free(semaphores);
            return 1;
        }
    }


    for (int i = 0; i < n; i++) {
        pthread_join(threads[i], NULL);
    }

  for (int i = 0; i < n; i++) {
        sem_destroy(&semaphores[i]);
    }
    free(semaphores);

    return 0;
}








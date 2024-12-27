#include <iostream>
#include <semaphore.h>
#include <pthread.h>
#include <atomic>

using namespace std;

void *reader(void *);
void *writer(void *);

atomic<int> readcount(0);
atomic<int> writecount(0);
int sh_var = 5;
sem_t x, y, rsem, wsem;

void *reader(void *i) {
    int id = *(int *)i;  // Cast to int pointer and dereference
    cout << "\n-------------------------";
    cout << "\nReader-" << id << " is waiting to read";
    
    sem_wait(&x);
    readcount++;
    if (readcount == 1) {
        sem_wait(&wsem);
    }
    sem_post(&x);
    
    cout << "\nReader-" << id << " is reading. Updated value: " << sh_var;

    sem_wait(&x);
    readcount--;
    if (readcount == 0) {
        sem_post(&wsem);
    }
    sem_post(&x);

    return nullptr;  // Ensure to return nullptr
}

void *writer(void *i) {
    int id = *(int *)i;  // Cast to int pointer and dereference
    cout << "\nWriter-" << id << " is waiting to write";

    sem_wait(&y);
    writecount++;
    if (writecount == 1) {
        sem_wait(&rsem);
    }
    sem_post(&y);
    
    sem_wait(&wsem);
    sh_var += 5;  // Write operation
    cout << "\nWriter-" << id << " has written. New value: " << sh_var;

    sem_post(&wsem);
    
    sem_wait(&y);
    writecount--;
    if (writecount == 0) {
        sem_post(&rsem);
    }
    sem_post(&y);

    return nullptr;  // Ensure to return nullptr
}

int main() {
    sem_init(&x, 0, 1);
    sem_init(&wsem, 0, 1);
    sem_init(&y, 0, 1);
    sem_init(&rsem, 0, 1);

    pthread_t r[5], w[2];
    int reader_ids[5] = {0, 1, 2, 3, 4};
    int writer_ids[2] = {0, 1};

    // Creating reader threads
    for (int i = 0; i < 5; ++i) {
        pthread_create(&r[i], NULL, reader, &reader_ids[i]);
    }

    // Creating writer threads
    for (int i = 0; i < 2; ++i) {
        pthread_create(&w[i], NULL, writer, &writer_ids[i]);
    }

    // Join threads
    for (int i = 0; i < 5; ++i) {
        pthread_join(r[i], NULL);
    }
    for (int i = 0; i < 2; ++i) {
        pthread_join(w[i], NULL);
    

    // Destroy semaphores
    sem_destroy(&x);
    sem_destroy(&wsem);
    sem_destroy(&y);
    sem_destroy(&rsem);

    return 0;
    system.out.
}

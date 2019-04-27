#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int *queue;
int size;
int space;
int head = 0;
int tail = 0;

FILE *fp;

pthread_mutex_t mutex;
pthread_cond_t queue_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t queue_not_empty = PTHREAD_COND_INITIALIZER;

void *task(void *arg);
void *cpu(void *arg);
void enqueue(int);
int dequeue(void);
void print_queue();

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: ./main <size>\n");
        return 1;
    }

    size = atoi(argv[1]);

    if (size < 1 || size > 10) {
        printf("Usage: ./main <1 - 10>");
        return 2;
    }

    space = size;

    queue = (int *) malloc(sizeof(int) * size);

    fp = fopen("task_list.txt", "r");

    if (fp == NULL) {
        printf("Error: failed to open file \"task_list\"\n");
        return 3;
    }

    pthread_t taskt, cpu_1, cpu_2, cpu_3;

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        printf("Error: mutex init has failed\n");
        return 4;
    }

    pthread_create(&taskt, NULL, task, NULL);
    pthread_create(&cpu_1, NULL, cpu, (void *) 1);
    pthread_create(&cpu_2, NULL, cpu, (void *) 2);
    pthread_create(&cpu_3, NULL, cpu, (void *) 3);

    pthread_join(taskt, NULL);
    pthread_join(cpu_1, NULL);
    pthread_join(cpu_2, NULL);
    pthread_join(cpu_3, NULL);

    pthread_mutex_destroy(&mutex);

    free(queue);
    fclose(fp);
    return 0;
}

void *task(void *arg) {
    while (!feof(fp)) {

        printf("TASK:\n");

        int cpu_burst;

        fscanf(fp, "%d", &cpu_burst);

        printf("\t+ %d\n", cpu_burst);

        enqueue(cpu_burst);
    }

    for (int i = 0; i < 3; i++)
        enqueue(-1);
}

void *cpu(void *arg) {
    int cpu_no = (int) arg;

    while (1) {
        printf("CPU #%d:\n", cpu_no);

        print_queue();

        int number = dequeue();

        printf("\t- %d\n", number);

        if (number < 0)
            break;

        sleep(number);
    }

    printf("CPU_%d: Exiting\n", cpu_no);
    return (NULL);
}

void enqueue(int cpu_burst) {
    pthread_mutex_lock(&mutex);

    while (space == 0)
        pthread_cond_wait(&queue_not_full, &mutex);

    pthread_mutex_unlock(&mutex);

    *(queue + (tail++ % size)) = cpu_burst;

    pthread_mutex_lock(&mutex);

    space--;

    pthread_cond_signal(&queue_not_empty);
    pthread_mutex_unlock(&mutex);
}

int dequeue(void) {
    pthread_mutex_lock(&mutex);

    while (space == size)
        pthread_cond_wait(&queue_not_empty, &mutex);

    pthread_mutex_unlock(&mutex);

    int cpu_burst = *(queue + (head++ % size));

    pthread_mutex_lock(&mutex);

    space++;

    pthread_cond_signal(&queue_not_full);
    pthread_mutex_unlock(&mutex);

    return cpu_burst;
}

void print_queue() {
    printf("\t");
    for (int i = 0; i < size; i++) {
        printf("-> %d ", *(queue + i));
    }
    printf("\n");
}

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 100
#define NUM_BUCKETS 10
#define NUM_THREADS 4

typedef struct {
    int *array;
    int start;
    int end;
    int **buckets;
} ThreadData;

int compareIntegers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

void *insertIntoBuckets(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int *array = data->array;
    int start = data->start;
    int end = data->end;
    int **buckets = data->buckets;

    for (int i = start; i < end; i++) {
        int bucket_index = array[i] / (ARRAY_SIZE / NUM_BUCKETS);
        int index = ++buckets[bucket_index][0];
        buckets[bucket_index][index] = array[i];
    }

    pthread_exit(NULL);
}

void gatherSortedElements(int *array, int **buckets) {
    int index = 0;
    for (int i = 0; i < NUM_BUCKETS; i++) {
        for (int j = 1; j <= buckets[i][0]; j++) {
            array[index++] = buckets[i][j];
        }
    }
}

void bucketsort(int arr[], int length) {
    int maximum = arr[0];
    for (int i = 1; i < length; i++) {
        if (arr[i] > maximum)
            maximum = arr[i];
    }

    int **buckets = (int **)malloc(sizeof(int *) * NUM_BUCKETS);
    for (int i = 0; i < NUM_BUCKETS; i++) {
        buckets[i] = (int *)malloc(sizeof(int) * ARRAY_SIZE);
        buckets[i][0] = 0; 
    }
    pthread_t threads[NUM_THREADS];
    ThreadData threadData[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadData[i].array = arr;
        threadData[i].start =i*(length/NUM_THREADS);
        threadData[i].end = (i + 1)*(length/NUM_THREADS);
        threadData[i].buckets = buckets;
        pthread_create(&threads[i], NULL,insertIntoBuckets,&threadData[i]);
    }
    //joining threadsd
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    //gather
    gatherSortedElements(arr, buckets);

 
}

int main() {
    int elements[] = {9, 5, 3, 5, 6, 7, 9, 4};
    int length = sizeof(elements) / sizeof(elements[0]);
  
    clock_t start = clock();
  
    bucketsort(elements, length);

    clock_t end = clock();
  
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("after sroting: \n");

    for(int i = 0; i < length; i++){
        printf("%d ", elements[i]);
    }

    printf("\ntime taken: %f seconds\n", time_taken);

    return 0;
}


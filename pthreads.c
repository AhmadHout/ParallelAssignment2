#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_VALUE 1000
#define NUM_BUCKETS 100


typedef struct {
    int* arr;         
    int* bucket;       
    int start;         
    int end;           
} specs;

void* buckethelper(void* arg) {
    specs* info = (specs*)arg;
    for (int i = info->start; i < info->end; i++) {
        info->bucket[info->arr[i]]++;
    }
    return NULL;
}
void bucketSort(int arr[], int size) {
    
    int bucket[MAX_VALUE + 1] = {0};
    int num_threads = 2; 
    pthread_t threads[num_threads];
    specs bucket_infos[num_threads];


    int chunk_size = size / num_threads;
    for (int i = 0; i < num_threads; i++) {
        bucket_infos[i].arr = arr;
        bucket_infos[i].bucket = bucket;
        bucket_infos[i].start = i * chunk_size;
        if (i == num_threads - 1) {
            bucket_infos[i].end = size;
        } else {
            bucket_infos[i].end = (i + 1) * chunk_size;
        }
        pthread_create(&threads[i], NULL, buckethelper, &bucket_infos[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    int index = 0;
    for (int i = 0; i <= MAX_VALUE; i++) {
        for (int j = 0; j < bucket[i]; j++) {
            arr[index++] = i;
        }
    }
}

int main() {
    srand(time(NULL));
    int* arr = malloc(NUM_BUCKETS* sizeof(int));
    if (arr == NULL) {
        perror("Memory allocation failed");
        return EXIT_FAILURE;
    }

    
    for (int i = 0; i<NUM_BUCKETS; i++) {
        arr[i] = rand() % MAX_VALUE;
    }

    
    clock_t start = clock();

  
    bucketSort(arr, NUM_BUCKETS);

 
    clock_t end = clock();

    
    double elapsed_time = ((double)(end - start)) / CLOCKS_PER_SEC;

    
    printf("Sorted Array:\n");
    for (int i = 0; i < NUM_BUCKETS; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nElapsed time: %f s\n", elapsed_time);
    free(arr);
    return 0;
}

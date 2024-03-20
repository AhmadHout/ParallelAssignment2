#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int Max(int arr[], int length){
  int max = arr[0];

  #pragma omp parallel for reduction(max:max)
  for (int i =1; i<length; i++){
    if (arr[i] > max)
      max = arr[i];
  }
  return max;
}

void bucketsort(int arr[], int length){
  
  int bucket[10];
  int maximum = Max(arr, length);

  #pragma omp parallel for
  for (int i = 0; i <= maximum; i++){
    bucket[i] = 0;
  }

  #pragma omp parallel for
  for (int i = 0; i < length; i++){
    #pragma omp atomic
    bucket[arr[i]]++;
  }

  int j=0;
  #pragma omp parallel for
  
  for (int i = 0; i <= maximum; i++){
    while (bucket[i] > 0){
      arr[j++] = i;
      #pragma omp atomic
      bucket[i]--;
    }
  }

}

int main(){
  int arr[] = {9, 5, 3, 5, 6, 7, 9, 4};
  int length = sizeof(arr) / sizeof(arr[0]);
  
  clock_t start = clock();
  
  bucketsort(arr, length);

  clock_t end = clock();
  
  double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
  printf("After sorting in ascending order: \n");

  for(int i = 0; i< length; i++){
    printf("%d ", arr[i]);
  }

  printf("\nTime taken: %f seconds\n", time_taken);

  return 0;
}

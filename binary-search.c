#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int GetRandom(int min, int max);
int binary_search(int array[], double ok, double ng, int k);

int main(void) {
  int n = 1000000;
  int array[n];

  array[0] = 3;
  srand((unsigned int)time(NULL));
  for(int i=0; i<n-1; i++) {
    array[i+1] = array[i] + GetRandom(1, 10);
  }

  int key = array[GetRandom(0, n)];

  int result = (binary_search(array, 0.0, n, key)) + 1;

  if(result > 0) {
    printf("%dは配列の%d番目にあります\n", key, result);
  } else {
    printf("%dは配列に存在しません\n", key);
  }
  //for(int i=0; i<n; i++){
  //  printf("%d\n", array[i]);
  //}

  return 0;
}

int binary_search(int array[], double ok, double ng, int k) {
  int mid;
  if(ok <= ng) {
    for(int i=0; i<100; i++){
      mid = (ok + ng) / 2.0;
    }

    if(array[mid] == k) {
      return mid;
    }
    else if(array[mid] > k) {
      return binary_search(array, ok, mid-1, k);
    }
    else if(array[mid] < k) {
      return binary_search(array, mid+1, ng, k);
    }
  }
  return -1;
}

int GetRandom(int min,int max){
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

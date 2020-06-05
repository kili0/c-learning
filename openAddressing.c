#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BacketSize 13
#define Delta 3
#define Num 11

//戻り値として利用する構造体を作成
typedef struct { int backet_num, data; } Hash;
Hash table[BacketSize];

int GetRandom(int min, int max);
void OpenAddressing(int data[], int m, int d);

int main(void) {
  int array[Num];
  srand((unsigned int)time(NULL));
  for(int i=0; i<Num; i++) {
    array[i] = GetRandom(1, 100);
    for(int j=0; j<i; j++) {
      if(array[i] == array[j]) {
        array[i] += GetRandom(-5, 5);
      }
    }
  }
  //test data
  //int array[Num] = {3, 18, 98, 24, 15, 11, 10, 8, 6, 5, 21};

  //ハッシュテーブル初期化
  for(int i=0; i<BacketSize; i++) {
    table[i].backet_num = i;
    table[i].data = 0;
  }

  OpenAddressing(array, BacketSize, Delta);

  for(int i=0; i<BacketSize; i++) {
    printf("%d : %d\n", table[i].backet_num, table[i].data);
  }
}

void OpenAddressing(int data[], int m, int d) {
  int flag[BacketSize];
  for(int i=0; i<BacketSize; i++) {
    flag[i] = 0;
  }

  for(int i=0; i<Num; i++){
    int b_num = data[i] % m;
    if(flag[b_num] == 0) {
      flag[b_num] = 1;
      table[b_num].data = data[i];
    } else {
      while(flag[b_num] != 0) {
        b_num += d;
        if(b_num > m) {
          b_num = b_num % m;
        }
      }
      flag[b_num] = 1;
      table[b_num].data = data[i];
    }
  }
}

int GetRandom(int min,int max){
	return min + (int)(rand()*(max-min+1.0)/(1.0+RAND_MAX));
}

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAXLINE 1000000
#define SIZE_TABLE 50

struct list {
  struct list *next;
  int key;
};

//hash
typedef struct node{
  int keyHash;
  struct list* list;
  struct node* pNextNode;
} node_t;

void printfHashTable(node_t** pHashTable){
	node_t* pNode = NULL;
	int i;

	for(i = 0; i < SIZE_TABLE; i++){
		if(pHashTable[i] != NULL){
			pNode = pHashTable[i];
			while(pNode != NULL){
				printf("hashVal:%d key:%d value:%p\n", i, pNode->keyHash, pNode->list);
				pNode = pNode->pNextNode;
			}
		}
	}
	printf("\n");
	return;
}

int hashFunc(int key){
	return key % SIZE_TABLE;
}

void initHashTable(node_t** pHashTable){
	for(int i=0; i<SIZE_TABLE; i++){
		pHashTable[i] = NULL;
	}
	printfHashTable(pHashTable);
}

node_t* initNode(int key, struct list *list){
	node_t* pNode = NULL;

	pNode = (node_t*)malloc(sizeof(node_t));
	if(pNode == NULL){
		printf("init malloc error\n");
		return NULL;
	}

	pNode->keyHash = key;
	pNode->list = list;
	pNode->pNextNode = NULL;

	return pNode;
}

void registerKey(node_t** pHashTable, int key, struct list *list){
	node_t* pNode = NULL;
	node_t* pPreNode = NULL;

	//連結リストが存在しない場合
	if(pHashTable[hashFunc(key)] == NULL){
		pHashTable[hashFunc(key)] = initNode(key, list);
		if(pHashTable[hashFunc(key)] == NULL){
			return;
		}
		return;
	}

	//連結リストが存在した場合
	pNode = pHashTable[hashFunc(key)];
	pPreNode = pNode;
	while(pNode != NULL){
		if(key == pNode->keyHash){
			pNode->list = list;
			return;
		}
		pPreNode = pNode;
		pNode = pNode->pNextNode;
	}

	pPreNode->pNextNode = initNode(key, list);

	return;
}

void search(node_t** pHashTable, int s_num) {
  if(pHashTable[hashFunc(s_num)] == NULL) {
    printf("Not found.\n");
  } else {
    node_t* pNode = pHashTable[hashFunc(s_num)];

    while(s_num != pNode->keyHash) {
      if(pNode->pNextNode == NULL){
        printf("Not found.\n");
        break;
      }
      pNode = pNode->pNextNode;
    }
    if(s_num == pNode->keyHash) {
      printf("%d -> hashVal:%d, list:%p\n", s_num, hashFunc(s_num), pNode->list);
    }
  }

  /* time */
  long cpu_time = clock();
  double sec = (double)cpu_time / CLOCKS_PER_SEC;
  printf("time: %f\n\n", sec);

  return;
}

int main() {
  node_t* pHashTable[SIZE_TABLE];
  initHashTable(pHashTable);

  struct list *p;
  struct list head;

  p = &head;
  int num = 0;

  for(int i=0; i<MAXLINE; i++){
    p -> next = malloc(sizeof(struct list));
    p = p->next;
    num = rand() % 100;
    p -> key = num;
    registerKey(pHashTable, p->key, p);
  }
  //printfHashTable(pHashTable);

  char command[16] = "";
  int s_num;
	while (strncmp(command, "exit", 4) != 0) {
		scanf("%s %d", command, &s_num);

		if(strncmp(command, "search", 6) == 0) {
			search(pHashTable, s_num);
		}
	}
  return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include "win32_timing.h"
#include "print_array.h"

#define cap 8

typedef struct {
	int prev;
	int next;
} Node;

typedef struct {
	int lru_index;
	int mru_index;
	int size;
	int cache[cap];
	Node linked_list[cap];
} LRU_Cache;

void PrintInt(const void* el) {
	assert(el);
	printf("%d", *(int*)el);
}

inline void FlushInputStream() {
	int ch;
	while((ch = getchar()) != '\n' && ch != EOF);
}

inline void ShowOptions() {
	printf("\n");
	printf("Press 'x' to exit!\n");
	printf("Enter a number between 1 and INT_MAX: ");
}

void LRU_Cache_Print(const LRU_Cache* lru_cache) {
	assert(lru_cache);
	
	printf("Cache contents: ");
	PrintArray(lru_cache->cache, lru_cache->size, sizeof(int), PrintInt);
	printf("LRU -> MRU: [");
	if(lru_cache->size > 0) {
		int index = lru_cache->lru_index;
		Node node = lru_cache->linked_list[index];
		while(node.next != -1) {
			printf("%d,", lru_cache->cache[index]);
			index = node.next;
			node = lru_cache->linked_list[index];
		}
		printf("%d", lru_cache->cache[index]);
	}

	printf("]\n");
}

void LRU_Cache_Init(LRU_Cache* lru_cache) {
	assert(lru_cache);

	lru_cache->lru_index = 0;
	lru_cache->mru_index = 0;
	lru_cache->size = 0;
	for(int i = 0; i < cap; ++i) {
		lru_cache->cache[i] = -1;
		lru_cache->linked_list[i].prev = -1;
		lru_cache->linked_list[i].next = -1;
	}
}

int GetItem(LRU_Cache* lru_cache, int val) {
	assert(lru_cache);
	assert(0 <= val);

	// search cache
	// if found:
	// 		update cache
	// 		return value
	for(int i = 0; i < lru_cache->size; ++i) {
		if(lru_cache->cache[i] == val) {
			// only update if val is not already MRU
			if(i != lru_cache->mru_index) {
				Node* mru_node     = &lru_cache->linked_list[lru_cache->mru_index];
				Node* current_node = &lru_cache->linked_list[i];
				Node* next_node    = &lru_cache->linked_list[current_node->next];

				// unlink current node from location
				next_node->prev = current_node->prev;
				if(i == lru_cache->lru_index) {
					lru_cache->lru_index = current_node->next;
				}
				else {
					Node* prev_node = &lru_cache->linked_list[current_node->prev];
					prev_node->next = current_node->next;
				}

				// update current mru node
				mru_node->next = i;
				
				// update current node to new MRU
				current_node->prev = lru_cache->mru_index;
				lru_cache->mru_index = i;
				current_node->next = -1;
			}

			printf("Found value %d in cache\n"
				   "Here you go! That was quick!\n", val);

			return lru_cache->cache[i];
		}
	}
	
	// not in cache
	// do slow retrieval
	printf("Could not find value %d in cache\n"
		   "Have to look elsewhere...\n", val);
	Sleep(3000);

	if(lru_cache->size < cap) {
		// insert at end of cache
		lru_cache->cache[lru_cache->size] = val;

		Node* current_node = &lru_cache->linked_list[lru_cache->size];
		Node* mru_node = &lru_cache->linked_list[lru_cache->mru_index];

		// NOTE: on the first insert we don't want the prev to be 0,
		// 		 so we check if it is LRU because lru_node->prev should
		// 		 always be -1, else we set prev to old mru_index and then
		// 		 update the mru_index
		current_node->prev = (lru_cache->size == lru_cache->lru_index) ? -1 : lru_cache->mru_index;
		lru_cache->mru_index = lru_cache->size;
		mru_node->next = lru_cache->size;
		current_node->next = -1;

		return lru_cache->cache[lru_cache->size++];
	}
	else {
		// evict lru
		lru_cache->cache[lru_cache->lru_index] = val;

		Node* mru_node     = &lru_cache->linked_list[lru_cache->mru_index];
		Node* current_node = &lru_cache->linked_list[lru_cache->lru_index];
		Node* next_node    = &lru_cache->linked_list[current_node->next];

		// update current mru node
		mru_node->next = lru_cache->lru_index;

		// update new lru
		next_node->prev = -1;
		lru_cache->lru_index = current_node->next;
		
		// update current node to new MRU
		current_node->prev = lru_cache->mru_index;
		lru_cache->mru_index = mru_node->next;
		current_node->next = -1;

		return lru_cache->cache[lru_cache->mru_index];
	}
}

int main(void) {

	LRU_Cache cache;
	LRU_Cache_Init(&cache);
	LRU_Cache_Print(&cache);


	while(true) {
		ShowOptions();
		char input[12];
		char* result = fgets(input, 12, stdin);
		if(result == NULL) {
			printf("Something went wrong...\n");
			return 1;
		}

		if(input[0] == 'x') {
			printf("exiting...\n");
			return 0;
		}


		int num = atoi(input);

		printf("Searching cache...\n");
		int val = GetItem(&cache, num);
		LRU_Cache_Print(&cache);

		//FlushInputStream();
	}


	return 0;
}

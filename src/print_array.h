#ifndef PRINT_ARRAY_H
#define PRINT_ARRAY_H

#include <stdio.h>

void PrintArray(const void* array,
		const int count,
		const int element_bytes,
		void(*PrintElement)(const void*)) 
{
	assert(array);
	assert(element_bytes > 0);
	assert(PrintElement);

	printf("[");
	int i;
	const char* element_ptr = (char*)array;
	for(i = 0; i < count - 1; ++i) {
		PrintElement(element_ptr + i*element_bytes);
		printf(", ");
	}
	if(count > 0) {
		PrintElement(element_ptr + i*element_bytes);
	}
	printf("]\n");
}


#endif

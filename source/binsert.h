#ifndef BINSERT_H
#define BINSERT_H

const void* binsert(const void* key, const void** plist, int size, int* nbelt, 
		    int* tablesize, int (*compar)(const void *, const void *));

#endif

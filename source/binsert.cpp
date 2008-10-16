#include <stdlib.h>
#include <string.h>

const void* binsert(const void* key, const void** plist, int size, int* nbelt, 
	     int* tablesize, int (*compar)(const void *, const void *))
{
  // first search to see if it exists already:
  const void* list = *plist;
  int a = 0;
  int b = *nbelt;
  while (a < b) {
    int c = (a+b)/2;
    int i = compar(key,((char*)list)+c*size);
    if (i < 0) b = c;
    else if (i > 0) a = c+1;
    else return ((char*)list)+c*size;
  }

  // insert the element
  (*nbelt)++;
  if (*nbelt*size > *tablesize) {
    *tablesize = *tablesize ? 2*(*tablesize) : 16*size;
    list = *plist = realloc(((char*)list), *tablesize);
  }
  //  printf("%d, %d, %d\n", *nbelt, a, *tablesize);
  if (*nbelt-1-a > 0)
    memmove(((char*)list)+(a+1)*size, ((char*)list)+a*size, (*nbelt-1-a)*size);
  memcpy(((char*)list)+a*size, key, size);
  return ((char*)list)+a*size;
}

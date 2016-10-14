
#ifndef _mymalloc_h_
#define _mymalloc_h_

#include <stdlib.h>
#include <stdio.h>

#define malloc( x ) mymalloc( x , __FILE__ , __LINE__ )
#define free( x ) myfree( x , __FILE__ , __LINE__ )



//linked list structure that is holding the allocations of memory used in our implementation of malloc and free.
//typedef struct MNodeLL MNodeLL;


void *mymalloc(unsigned int size, char *file, int line);
void myfree(void *p, char *file, int line);

#endif

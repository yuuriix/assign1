#include <stdio.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

#if 1
	#define PRINT(a) printf a
#else
	#define PRINT(a) (void) 0
#endif

// global varibales track total free space, allocated space, largest space, and global address
static char myblock[65000];
int memfree = 65000;
int memalloc = 0;
void *location = myblock;
int largestblk = 65000;
void *globalptr = myblock; // global pointer to location in myblock


// mymalloc function

void *mymalloc(int size){


    /* 1) ERROR CHECKING */


   // 1a) Check for positive size
   if(size<0){
       fprintf(stderr, "Error: Size request must be at least 0\n");
   }

   // 1b) Check to see if space is available
   if(memfree<size || largestblk<size){
       if(largestblk<size && memfree>=size){
           fprintf(stderr, "Fragmentation Error: Available Free space split\n");
       }else{
           fprintf(stderr, "Error: Size request exceeds available free space\n");
       }
   }


   /* 2) INITIALIZING BLOCK */
    

   // 2a) Get size of metadata and block
   int blocksize = sizeof(int)+size+sizeof(char);
   int metasize = sizeof(int)+sizeof(char); 
   
   // 2b) Find first empty block
   int start = *(int*)globalptr;
	if(start>0 || (blocksize)>(*(int*)globalptr)){
		globalptr = globalptr+start;
		while((*(int*)globalptr)>0){
			globalptr = globalptr+(*(int*)globalptr);
		}
	}
	PRINT(("\nmyblock location check in 2b): %p\n",location));
   
   // 2c) Store size of block, so it can be added to globalptr for next block address
   int * blkptr = (int *)globalptr;
   *blkptr = blocksize;
   PRINT(("Location of globalptr 2c): %p\n",globalptr));
   PRINT(("Block size is: %d\n",blocksize));
   PRINT(("Stored size is: %d\n\n",*(int*)globalptr));
   globalptr = globalptr+sizeof(int);
   
   // 2d) Get pointer to data location. This is the returned ptr
   PRINT(("Location of globalptr 2d): %p\n",globalptr));
   void *retptr = (void*)globalptr;
   globalptr = globalptr+size;
   
   // 2e) Set allocation flag to 1 for allocated
   PRINT(("Location of globalptr 2e): %p\n\n",globalptr));
   *((char*)globalptr) = '1';
   PRINT(("Allocflag: %c\n",*(char*)globalptr));
   
   // 2f) Update global variables
   memfree = memfree-blocksize;
   PRINT(("Free Memory Left: %d\n",memfree));
   memalloc = memalloc+blocksize;
   PRINT(("Total Memory Allocated: %d\n",memalloc));
   globalptr = location;
   PRINT(("Location of globalptr in 2g): %p\n\n",globalptr));
   
   // 2g) Return writeable address
   return retptr;
   
}

int main(void){
	
	// first allocation test
	char *blah = (char*)mymalloc(sizeof(char));
	*blah = 't';
	PRINT(("Location of Blah is: %p\n",blah));
	printf("Char in blah is: %c\n\n",*blah);
	
	// second allocation test
	int *mynum = (int*)mymalloc(sizeof(int));
	*mynum = 11;
	PRINT(("Location of mynum is: %p\n",mynum));
	printf("Int in mynum is: %d\n\n",*mynum);
	
	
	return 0;
	
}

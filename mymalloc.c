#include "mymalloc.h"

#define Blocksize 100000



// Node structure contains allocation flag and size of data

struct MNodeLL
{
	struct MNodeLL *prev, *next;
	int alloc;		// 1 - yes, 0 - no
	int size;
};



// initialize global variables for, "heap", size of each block (numBlocks), and the ptr to the first node (NodeArr)

static char myblock[Blocksize]; //big block of memory space
static const int numBlocks = Blocksize/sizeof(struct MNodeLL)+1; //size of NodeArr
static void *NodeArr[Blocksize/sizeof(struct MNodeLL)+1] = {0}; //pointers to NodeArr



// find first index that doesn't have MNodeLL

static int FreeFirstAlg() {
	
	int i;
	
	// find first index without
	for (i = 0; i < numBlocks; i++)
		if (NodeArr[i] == 0) 
			return i;
	return 1; 
	
}



// gives back a pointer leading to the allocated block of memory

void *mymalloc(unsigned int size, char *file, int line){
	
	// declare tracker variables for mymalloc
	static int start = 0;
	struct MNodeLL *p;
	struct MNodeLL *next;
	static struct MNodeLL *first;
	
	// Error 1 of 2: Asking to allocate something less than size 0
	if (size<0){
		
		fprintf(stderr, "ERROR: Cannot allocate negative bytes in FILE: '%s' on LINE: '%d'\n", file, line);
		return 0;
		
	}
	
	// allocating 0 results in returning null
	if (size==0){
		
		return NULL;
		
	}
	
	// First time called, so create first lump
	if(!start){
		
		// creates a initial lump at the start of the memory block
		first = (struct MNodeLL*)myblock; // myblock to set equal to first
		first->prev = first->next = 0;
		first->size = Blocksize - sizeof(struct MNodeLL);
		first->alloc = 1;
		start = 1; // indicate that it has already been called to create node
		NodeArr[FreeFirstAlg()] = myblock;
		
	}

	p = first;
	// expand or manipulate LL around first as needed
	do{
			
			
		// Current lump is tinier, go to the succeeding lump
		// Or this lump was stolen (aka already taken), go to the next
		
		if(p->size < size || !p->alloc) {
			
			p = p->next;
			
		}else if(p->size<(size+sizeof(struct MNodeLL))){
			
			
			// this lump is free/not allocated and is wide enough to hold data, but there's not enough memory to hold the HEADER of the next lump of memory
			// don't create any more lumps of memory after this
			
			p->alloc = 0;
			
			return (char*)p + sizeof(struct MNodeLL);
			
		}else{
			
			
			// take the needed memory and initialize the next lump of memory
			
			// position next node in doubly linked list relative to first
			next = (struct MNodeLL*)((char*)p + sizeof(struct MNodeLL) + size);
			next->prev = p;
			next->next = p->next;
			next->size = p->size - sizeof(struct MNodeLL) - size;
			
			// change attributes of p
			next->alloc = 1; // change allocation flag
			NodeArr[FreeFirstAlg()] = next;
			p->size = size;
			p->alloc = 0;
			p->next = next;
			
			// return ptr to user
			return (char*)p + sizeof(struct MNodeLL);
			
		}
	} while (p != 0);

	// Error 2 of 2: Trying to allocate something when you've run out of memory
	fprintf(stderr, "ERROR: Not enough memory remaining (%d bytes) in FILE: '%s' on LINE: '%d'\n", size, file, line);
	
	return 0;
	
}



// free a memory buffer pointed to by p

void myfree(void *p, char *file, int line)
{
	
	// declare current, prev, and next ptrs
	struct MNodeLL *ptr;
	struct MNodeLL *prev;
	struct MNodeLL *next;

	// Error 1 in Free: pointers pointing to nothing (NULL) cannot be freed
	if (p == NULL) {
		fprintf(stderr, "ERROR: Cannot free pointer to NULL in file, free failed in FILE: '%s' on LINE: '%d'\n", file, line);
		return;
	}

	// change ptr's location
	ptr = (struct MNodeLL*)((char*)p-sizeof(struct MNodeLL));
	
	
	//check if MNodeLL ptr is allocated
	int i;
	int isAlloc = 0;
	
	for(i=0; i<numBlocks; i++){
		if(ptr==NodeArr[i] && !ptr->alloc){
			
			isAlloc = 1; //MNodeLL is Allocated
			break;
			
		}
	}
	
	
	// Error 2 in Free: freeing memory that was never allocated
	if(!isAlloc){
		
		fprintf(stderr, "ERROR: Cannot free memory that was not malloced in FILE: '%s' on LINE: '%d'\n", file, line);
		return;
		
	}

	if((prev = ptr->prev) != 0 && prev->alloc){
		
		// the previous lump is free so please merge this lump with the previous
		prev->size += sizeof(struct MNodeLL) + ptr->size;
		NodeArr[i] = 0; //merged with previous, so removing free MNodeLL
		
	}else{
		
		//not setting MNodeLL to null but that doesn't necessarily mean removing it, all it means is that alloc=1
		ptr->alloc = 1;
		prev = ptr;	// used for the step below
		
	}
	
	
	if((next=ptr->next) != 0 && next->alloc){
		
		// the next lump is free, merge with it
		prev->size += sizeof(struct MNodeLL) + next->size;
		prev->next = next->next;
		
		//prev->alloc = 1;
		for (i=0; i <numBlocks; i++){
			if (next==NodeArr[i]){
				
				NodeArr[i] = 0; //merged with next, so removing free MNodeLL
				break;
				
			}
		}
	}
}

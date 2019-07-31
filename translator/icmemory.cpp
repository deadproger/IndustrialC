#include "icmemory.h"
#include <iostream>
#include <set>
#include <typeinfo>

//=================================================================================================
//Memory leak tracker
//keeps track of how many pointers are have allocated memory
//The number is output at the end of program to check for memory leaks
//=================================================================================================
#ifdef ICDEBUG

unsigned long ptrs_index = 0;
void* ptrs[0xffffff];

unsigned long num_allocated_ptrs = 0;
inline void* ic_mem_new(std::size_t sz)
{
	void *ptr = std::malloc(sz);
	if(NULL == ptr)
	{
		std::cout<<"Bad alloc: trying to allocate "<<sz<<"bytes"<<std::endl;
		exit(-1);
	}
	num_allocated_ptrs++;

	for(unsigned long i = 0;i<ptrs_index;i++)
	{
		if(ptr == ptrs[i])
			ptrs[i] = NULL;
	}
	
	return ptr;
}
inline void ic_mem_delete(void* ptr)
{
	if(NULL == ptr) 
	{
		std::cout<<"deleting NULL"<<std::endl;
		return; 
	}
	num_allocated_ptrs--; 
	
	for(unsigned long i = 0;i<ptrs_index;i++)
	{
		if(ptr == ptrs[i])
			std::cout<<"deleting twice "<<ptr<<std::endl;
	}
	ptrs[ptrs_index++] = ptr;

	std::free(ptr);
}

//for C++11+ comment out the throws
void* operator new(std::size_t sz)/*throw (std::bad_alloc)*/ {return ic_mem_new(sz);} //modify these lines
void* operator new[](std::size_t sz)/*throw (std::bad_alloc)*/ {return ic_mem_new(sz);} //modify these lines
void operator delete(void* ptr)throw (){ic_mem_delete(ptr);} //these lines shouldn't be modified
void operator delete[](void* ptr)throw (){ic_mem_delete(ptr);} //these lines shouldn't be modified

#endif /*ICDEBUG*/

unsigned long get_num_allocs()
{
#ifdef ICDEBUG
	return num_allocated_ptrs;
#else
	return 0;
#endif /*ICDEBUG*/
}

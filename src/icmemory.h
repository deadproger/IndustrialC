#pragma once

#include <cstdio>
#include <cstdlib>
#include <new>

//#define ICDEBUG

//Pseudo assertion - prints a message to std::cout
#ifdef ICDEBUG
#define ICASSERT(a) if(!(a)){std::cout<<__FILE__<<": line "<<__LINE__<<": Assertion failure"<<std::endl;}
#else
#define ICASSERT(a)
#endif

inline void ic_mem_delete(void* ptr);

//=================================================================================================
//Memory leak tracker
//=================================================================================================
#ifdef ICDEBUG
void* operator new(std::size_t sz)/*throw (std::bad_alloc)*/;
void* operator new[](std::size_t sz)/*throw (std::bad_alloc)*/;
void operator delete(void* ptr)throw ();
void operator delete[](void* ptr)throw ();
#endif /*ICDEBUG*/
unsigned long get_num_allocs();
//=================================================================================================
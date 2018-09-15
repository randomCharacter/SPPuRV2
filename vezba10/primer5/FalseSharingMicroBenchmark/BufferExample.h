#pragma once

#include "Defines.h"

#include <tbb/task.h>
#include <tbb/scalable_allocator.h>
#include <tbb/cache_aligned_allocator.h>
#include <tbb/tick_count.h>

#include <iostream>

using namespace tbb;

using namespace std;



class BufferExample: public task{

private:
	/// Object size in bytes.
	unsigned int uiObjectSize;
	
	/// Number of loop iteration.
	unsigned int uinLoopIterations;

	/// Unique thread ID.
	unsigned char ucId;

	/// allocator type: scallable allocator, scallable aligned allocator or default allocator.
	unsigned char ucMallocType;

	/// Pointer to pdResults array.
	double * pdResults;

public:
	BufferExample(unsigned int uiObjectSize_, unsigned int uinLoopIterations_, unsigned char ucId_, unsigned char ucMallocType_, double * results_):
		uiObjectSize(uiObjectSize_), uinLoopIterations(uinLoopIterations_), ucId(ucId_), ucMallocType(ucMallocType_), pdResults(results_)
	{
	
	}

	task * execute(){
		// memory pointer
		unsigned char * pucPointer;
		
		// some temporaly value
		unsigned char ucTemp;

		// measure thread execution time
		tick_count start = tick_count::now();

		// TODO: allocate memory (depending on malloc type and  object size)
		switch(ucMallocType){
			case __SCALABLE_ALLOCATOR__:{
				//pucPointer = ?
				break;
			}
			case __SCALABLE_ALIGN_ALLOCATOR__:{
				 //
				 break;
			}
			default:{
				 //
				 break;
			}
		}

		// do memory access
		for(unsigned int i = 0; i < uinLoopIterations; i++){	
			for(unsigned int j = 0; j < uiObjectSize; j++){
				pucPointer[j] = (unsigned char) j;
				ucTemp = pucPointer[j];
			}	
		}

		// TODO: free memory (depending on malloc type and  object size)

		// measure thread execution time
		tick_count end = tick_count::now();
		// and write result in array
		pdResults[ucId] = (end - start).seconds();

		return NULL;
	}
};
#pragma once

#include "BufferExample.h"
#include "StlExample.h"
#include "Defines.h"

#include <tbb/task.h>
#include <tbb/spin_mutex.h>
#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#include <math.h>

using namespace tbb;

class Processing: public task{

private:

	/// Describe wow much threads is using in program.
	unsigned int uiNumberOfThreads;

	/// Object size in bytes.
	unsigned int uiObjectSize;
	
	/// Number of loop iteration.
	unsigned int uinLoopIterations;

	/// Unique thread ID.
	unsigned char ucId;

	/// allocator type: scallable allocator, scallable aligned allocator or default allocator.
	unsigned char ucMallocType;

	/// Pointer to results array.
	double * pdResults;

	/// Array pointer to BufferExample object type. Threads (Tasks) are executed in this objects.
	BufferExample * bufferExample[__MAX_THREADS__];

	/// Array pointer to StlExample object type. Threads (Tasks) are executed in this objects.
	StlExample * stlExample[__MAX_THREADS__];

	/// Example type: BUFFER or STL example.
	unsigned char ucExampleType;

public:
	Processing(unsigned int uiNumberOfThreads_, unsigned int uiObjectSize_, unsigned int uinLoopIterations_, unsigned char ucMallocType_, double * pdResults_, unsigned char ucExampleType_):
	uiNumberOfThreads(uiNumberOfThreads_), uiObjectSize(uiObjectSize_), uinLoopIterations(uinLoopIterations_), ucMallocType(ucMallocType_), pdResults(pdResults_), ucExampleType(ucExampleType_)
	{
		task_scheduler_init init;
	}



	task * execute(){

		// allocate threads (tasks)
		for(unsigned int i = 0; i < uiNumberOfThreads; i++){
			if(ucExampleType == __BUFFER__){
				bufferExample[i] = new (this->allocate_child()) BufferExample(uiObjectSize, uinLoopIterations, i, ucMallocType, pdResults);
			}
			else{
				stlExample[i] = new (this->allocate_child()) StlExample(uiObjectSize, uinLoopIterations, i, ucMallocType, pdResults);
			}
		}

		// choose pattern: Blocking style with Children
		set_ref_count(uiNumberOfThreads + 1);

		// spawn tasks
		for(unsigned int i = 0; i < uiNumberOfThreads - 1; i++){
			if(ucExampleType == __BUFFER__){
				this->spawn(*bufferExample[i]);
			}
			else{
				this->spawn(*stlExample[i]);
			}
		}

		// wait for all tasks
		if(ucExampleType == __BUFFER__){
			this->spawn_and_wait_for_all(*bufferExample[uiNumberOfThreads - 1]);
		}
		else{
			this->spawn_and_wait_for_all(*stlExample[uiNumberOfThreads - 1]);
		}

		return NULL;
	}
};


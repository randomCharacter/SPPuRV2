#pragma once

#include "Defines.h"

#include <tbb/task.h>
#include <tbb/scalable_allocator.h>
#include <tbb/cache_aligned_allocator.h>
#include <tbb/tick_count.h>

#include <iostream>
#include <vector>

using namespace tbb;

using namespace std;



class StlExample: public task{

private:
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

public:
	StlExample(unsigned int uiObjectSize_, unsigned int uinLoopIterations_, unsigned char ucId_, unsigned char ucMallocType_, double * pdResults_):
		uiObjectSize(uiObjectSize_), uinLoopIterations(uinLoopIterations_), ucId(ucId_), ucMallocType(ucMallocType_), pdResults(pdResults_)
	{

	}

	task * execute(){
		// some temporaly value
		unsigned char ucTemp;

		// measure thread execution time
		tick_count start = tick_count::now();

		// allocate memory
		switch(ucMallocType){
			case __SCALABLE_ALLOCATOR__:{
				vector<int, scalable_allocator<int> > v;

				for(unsigned int i = 0; i < uinLoopIterations; i++){
					for(unsigned int j = 0; j < uiObjectSize; j++){
						// if vector is empty, fill it with values
						if(j >= v.size()){
							v.push_back(j);
						}
						else{
							// just override vector value
							v[j] = (unsigned char) j;
						}
						ucTemp = v[j];
					}
				}
				break;
			}
			case __SCALABLE_ALIGN_ALLOCATOR__:{
				 vector<int, cache_aligned_allocator<int> > v;
				 for(unsigned int i = 0; i < uinLoopIterations; i++){
					for(unsigned int j = 0; j < uiObjectSize; j++){
						// if vector is empty, fill it with values
						if(j >= v.size()){
							v.push_back(j);
						}
						else{
							// just override vector value
							v[j] = (unsigned char) j;
						}
						ucTemp = v[j];
					}
				}
				 break;
			}
			default:{
				 vector<int> v;
				 for(unsigned int i = 0; i < uinLoopIterations; i++){
					for(unsigned int j = 0; j < uiObjectSize; j++){
						// if vector is empty, fill it with values
						if(j >= v.size()){
							v.push_back(j);
						}
						else{
							// just override vector value
							v[j] = (unsigned char) j;
						}
						ucTemp = v[j];
					}
				}
				 break;
			}
		}

		// measure thread execution time
		tick_count end = tick_count::now();
		// and write result in array
		pdResults[ucId] = (end - start).seconds();

		return NULL;
	}
};

#include "Main.h"
#include "Defines.h"
#include "Processing.h"

#include <tbb/task.h>
#include <tbb/tick_count.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>


using namespace tbb;

using namespace std;

/// Describe wow much threads is using in program.
unsigned int uiNumberOfThreads;

/// Object size in bytes.
unsigned int uiObjectSize;

/// Number of loop iteration.
unsigned int uinLoopIterations;

/// Execution time for each thread.
double pdResults[__MAX_THREADS__];


/**
* @brief main function
* @param argc number of arguments
* @param argv array of arguments values,
*				argv[1] = number of threads
*				argv[2] = object size
*				argv[3] = loop iterations
*/
int main(int argc, char *argv[]){

	if(argc == __ARG_NUM__){
		uiNumberOfThreads = atoi(argv[1]);
		uiObjectSize = atoi(argv[2]);
		uinLoopIterations = atoi(argv[3]);

		cout << "Threads number: " << uiNumberOfThreads << "\n";
		cout << "Object size: " << uiObjectSize << "\n";
		cout << "Loop iterations: " << uinLoopIterations << "\n\n";

		cout << "Buffer example" << "\n";
		test(__REGULAR_ALLOCATOR__, __BUFFER__);
		test(__SCALABLE_ALLOCATOR__, __BUFFER__);
		test(__SCALABLE_ALIGN_ALLOCATOR__, __BUFFER__);

		cout << "STL example" << "\n";
		test(__REGULAR_ALLOCATOR__, __STL__);
		test(__SCALABLE_ALLOCATOR__, __STL__);
		test(__SCALABLE_ALIGN_ALLOCATOR__, __STL__);
	}
	else{
		cout << "ERROR: bad arguments number, call program like: FalseSharingMicroBenchmark.exe"
			<< " NUMBER_OF_THREADS"
			<< " OBJECT_SIZE"
			<< " LOOP_ITERATION"
			<< "\n";
	}
	return 0;
}


void test(unsigned char ucMallocType, unsigned char ucExampleType){

	switch(ucMallocType){
		case __SCALABLE_ALLOCATOR__:{
			cout << "Using scalable alocator" << "\n";
			break;
		}
		case __SCALABLE_ALIGN_ALLOCATOR__:{
			 cout << "Using scalable aligned alocator" << "\n";
			 break;
		}
		default:{
			 cout << "Using regular allocator" << "\n";
			 break;
		}
	}

	// measure total execution time
	tick_count start = tick_count::now();

	Processing &processing = *new(task::allocate_root())Processing(uiNumberOfThreads, uiObjectSize, uinLoopIterations, ucMallocType, pdResults, ucExampleType);
	task::spawn_root_and_wait(processing);

	tick_count end = tick_count::now();

	// get results for each thread
	for(unsigned int i = 0; i < uiNumberOfThreads; i++){
		cout << "Execution time for thread [" << i + 0 << "]: " << pdResults[i] << " seconds." << "\n";
	}
	cout << "Total execution time: " << (end - start).seconds() << " seconds." << "\n";
	cout << "Density: " << calculateDensity() << "\n\n";
}

double calculateDensity(){
	double res = 0;

	for(unsigned int i = 0; i < uiNumberOfThreads; i++){
		for(unsigned int j = 0; j < uiNumberOfThreads; j++){
			if(i != j){
				res += fabs(pdResults[i] - pdResults[j]);
			}
		}
	}

	return res;
}

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/task.h"

using namespace tbb;
using namespace std;

#define WORKLOAD 100000000

void do_work() {
	for( int i=0; i<WORKLOAD; i++ ){
		;
	}
}

class Dummy: public tbb::task {
public:
	tbb::task* execute( ) {
		cout << "Hello from execute()!" << endl;
		do_work();
		return NULL;
	}
};

void serial() {

	//ToDo: Replace with actual tasks	
	Dummy *t1 = new( tbb::task::allocate_root( ) ) Dummy();
	t1->execute();
	Dummy *t2 = new( tbb::task::allocate_root( ) ) Dummy();
	t2->execute();
	Dummy *t3 = new( tbb::task::allocate_root( ) ) Dummy();
	t3->execute();
	return;
}

void BuildAndEvaluateDAG() {	

	//ToDo: Add code here

	return;
}

void parallel() {

	BuildAndEvaluateDAG();
	return;
}

int main( int argc, char* argv[] ) {

	tick_count startTime = tick_count::now();
	serial();
	tick_count stopTime = tick_count::now();
	cout << "serial finished. Took: " << 
		(stopTime - startTime).seconds()*1000 << "ms." << endl;

	startTime = tick_count::now();
	parallel();
	stopTime = tick_count::now();
	cout << "parallel finished. Took: " << 
		(stopTime - startTime).seconds()*1000 << "ms." << endl;	

	return 0;
}
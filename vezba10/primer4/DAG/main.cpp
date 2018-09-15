#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/task.h"

using namespace tbb;
using namespace std;

const int M=3, N=3; 

class DagTask: public tbb::task { 
public: 
	const int i, j; 
	// input[0] = sum from above, input[1] = sum from left 
	double input[2]; 
	double sum; 
	// successor[0] = successor below, successor[1] = successor to right 
	DagTask* successor[2]; 
	DagTask( int i_, int j_ ) : i(i_), j(j_) { 
		input[0] = input[1] = 0; 
	} 
	task* execute() { 
		__TBB_ASSERT( ref_count()==0, NULL ); 
		sum = i==0 && j==0 ? 1 : input[0]+input[1]; 
		for( int k=0; k<2; ++k ) 
			if( DagTask* t = successor[k] ) { 
				t->input[k] = sum; 			
				if( t->decrement_ref_count()==0 ) 
					spawn( *t ); 
			} 

			printf("[DEBUG][%s] i=%d j=%d input[0]=%.2f, input[1]=%.2f, sum= %.2f\n",
				__FUNCTION__, i, j, input[0], input[1], sum);//!

			return NULL; 
	} 
};

double BuildAndEvaluateDAG() { 
	DagTask* x[M][N]; 
	for( int i=M; --i>=0; ) 
		for( int j=N; --j>=0; ) {
			x[i][j] = new( tbb::task::allocate_root() ) DagTask(i,j); 
			x[i][j]->successor[0] = i+1<M ? x[i+1][j] : NULL; 
			x[i][j]->successor[1] = j+1<N ? x[i][j+1] : NULL; 
			x[i][j]->set_ref_count((i>0)+(j>0)); 
		} 
		// Add extra reference to last task, because it is waited on 
		// by spawn_and_wait_for_all. 
		x[M-1][N-1]->increment_ref_count(); 
		// Wait for all but last task to complete. 
		x[M-1][N-1]->spawn_and_wait_for_all(*x[0][0]);
		// Last task is not executed implicitly, so execute it explicitly. 
		x[M-1][N-1]->execute(); 
		double result = x[M-1][N-1]->sum; 
		// Destroy last task. 
		task::destroy(*x[M-1][N-1]); 
		return result; 
}

int main(int argc, char* argv[])
{
	double sum = -1;
	// build and evaluate DAG
	sum = BuildAndEvaluateDAG();
	printf("sum = %.2f\n", sum);
	return 0;
}
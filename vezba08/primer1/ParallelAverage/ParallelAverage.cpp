#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"
using namespace tbb;

struct Average {
	float* input;
	float* output;
	void operator()( const blocked_range<int>& range ) const {
		for (int i=range.begin(); i!=range.end(); ++i)
			output[i] = (input[i-1]+input[i]+input[i+1])*(1/3.0f);
	}
};

// Note: The input must be padded such that input[-1] and 
// input[n] can be used to calculate the first and last 
// output values.
void ParallelAverage( float* output, float* input, size_t n) {
	Average avg;
	avg.input = input;
	avg.output = output;
	parallel_for( blocked_range<int>(0, n, 1000), avg);
}

// Sets output[i] to the average of input[i-1], input[i] and input[i+1]
int main() {
	float input[1002], output[1000];

	input[0] = 0; 
	input[1001] = 0;
	for (int i = 1; i < 1001; i++) {
		input[i] = i * 3.14;
	}

	ParallelAverage(output, &(input[1]), 1000);


	return 0;
}

#include <stdio.h>
#include <float.h>

#include "tbb/tbb.h"
#include "tbb/parallel_reduce.h"

using namespace tbb;

#define NUM_OF_ELEMENTS 100

float Foo(float a) {
	return a;
}

class MinIndexFoo {
	const float *const my_a;
public:
	float value_of_min;
	long index_of_min;
	void operator()(const blocked_range<size_t>& r) {
		const float *a = my_a;
		for (size_t i = r.begin(); i != r.end(); ++i) {
			float value = Foo(a[i]);
			if (value < value_of_min) {
				value_of_min = value;
				index_of_min = i;
			}
		}
	}

	MinIndexFoo (MinIndexFoo& x, split) :
		my_a(x.my_a),
		value_of_min(FLT_MAX),
		index_of_min(-1)
	{}

	void join (const MinIndexFoo& y) {
		if (y.value_of_min < value_of_min) {
			value_of_min = y.value_of_min;
			index_of_min = y.index_of_min;
		}
	}

	MinIndexFoo (const float a[]) :
		my_a(a),
		value_of_min(FLT_MAX),
		index_of_min(-1)
	{}

};

long ParallelMinIndexFoo (float a[], size_t n) {
	MinIndexFoo mif(a);
	parallel_reduce(blocked_range<size_t>(0,n), mif);
	return mif.index_of_min;
}

int main () {
	float a[NUM_OF_ELEMENTS];

	for (int i=0; i<NUM_OF_ELEMENTS; ++i) {
		a[i] = -i;
	}

	long ind = ParallelMinIndexFoo(a, NUM_OF_ELEMENTS);
	printf("%d", ind);

}

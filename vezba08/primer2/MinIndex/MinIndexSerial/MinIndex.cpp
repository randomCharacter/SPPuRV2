#include <stdio.h>
#include <float.h>

#define NUM_OF_ELEMENTS 100

float Foo(float a) {
	return a;
}

long SerialMinIndexFoo (const float a[], size_t n) {
	float value_of_min = FLT_MAX;   // FLT_MAX from <float.h>
	long index_of_min = -1;
	for( size_t i=0; i<n; ++i ){
		float value = Foo(a[i]);
		if (value < value_of_min) {
			value_of_min = value;
			index_of_min = i;
		}
	}
	return index_of_min;
}

int main () {
	float a[NUM_OF_ELEMENTS];

	for (int i=0; i<NUM_OF_ELEMENTS; ++i) {
		a[i] = (float)-i;
	}

	long ind = SerialMinIndexFoo(a, NUM_OF_ELEMENTS);
	printf("%d", ind);

}

#include "tbb/tbb.h"
#include "MyBuffer.h"

// Filter that changes the first letter of each word
// from lowercase to uppercase
class MyTransformFilter: public tbb::filter {
public:
	MyTransformFilter();
	/*override*/void* operator()(void* item);
};

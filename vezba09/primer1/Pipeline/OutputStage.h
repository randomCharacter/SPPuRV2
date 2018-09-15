#include "tbb/tbb.h"
#include "MyBuffer.h"

class MyOutputFilter: public tbb::filter {
public:
	MyOutputFilter(FILE* output_file_);
private:
	FILE* output_file;
	/*override*/
	void* operator()(void* item);
};

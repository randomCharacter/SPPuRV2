#include "tbb/tbb.h"
#include "MyBuffer.h"

class MyInputFilter: public tbb::filter {
public:
	static const size_t n_buffer = 4;
	MyInputFilter(FILE* input_file);
private:
	FILE* input_file;
	size_t next_buffer;
	char last_char_of_previous_buffer;
	MyBuffer buffer[n_buffer];
	/*override*/ void* operator()(void*);
};

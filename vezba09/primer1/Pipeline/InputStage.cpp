#include "InputStage.h"

MyInputFilter::MyInputFilter(FILE* input_file_) :
	filter(/*is_serial=*/true),
	input_file(input_file_),
	next_buffer(0),
	last_char_of_previous_buffer(' ')
{}

void* MyInputFilter::operator()(void*) {
	MyBuffer& b = buffer[next_buffer];
	next_buffer = (next_buffer+1) % n_buffer;
	size_t n = fread(b.begin(), 1, b.max_size(), input_file);
	if (!n) {
		// end of file
		return NULL;
	} else {
		b.begin()[-1] = last_char_of_previous_buffer;
		last_char_of_previous_buffer = b.begin()[n-1];
		b.set_end(b.begin()+n);
		return &b;
	}
}

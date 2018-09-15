#include "OutputStage.h"

MyOutputFilter::MyOutputFilter(FILE* output_file_) :
	filter(/*is_serial=*/true),
	output_file(output_file_)
{}

void* MyOutputFilter::operator()(void* item) {
	MyBuffer& b = *static_cast<MyBuffer*>(item);
	fwrite(b.begin(), 1, b.size(), output_file);
    return NULL;
}

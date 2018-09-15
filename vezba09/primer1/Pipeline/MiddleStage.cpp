#include "MiddleStage.h"

MyTransformFilter::MyTransformFilter():
	tbb::filter(/*serial=*/false)
{}

/*override*/void* MyTransformFilter::operator ()(void* item){
	MyBuffer& b = *static_cast<MyBuffer*>(item);
	bool prev_char_is_space = b.begin()[-1]==' ';
	for (char* s=b.begin(); s!=b.end(); ++s) {
		if (prev_char_is_space && islower(*s))
			*s = toupper(*s);
		prev_char_is_space = isspace(*s);
	}
	return &b;
}
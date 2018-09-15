#ifndef _MY_BUFFER_
#define _MY_BUFFER_

#include <stdio.h>

class MyBuffer {
private:
	static const size_t buffer_size = 10000;
	char* my_end;
	// storage[0] holds the last character of the preceding buffer
	char storage[1+buffer_size];
public:
	// pointer to the first character in the burffer
	char* begin() {return storage+1;}
	const char* begin() const {return storage+1;}
	// pointer to one past last character in the buffer
	char* end() const {return my_end;}
	// set end of buffer
	void set_end(char* new_ptr) {my_end=new_ptr;}
	// number of bytes a buffer can hold
	size_t max_size() const {return buffer_size;}
	// number of bytes in buffer
	size_t size() const {return my_end-begin();}
};

#endif //_MY_BUFFER_

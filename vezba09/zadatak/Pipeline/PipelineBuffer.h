#ifndef _PIPELINE_BUFFER_H_
#define _PIPELINE_BUFFER_H_

#include "defines.h"

// Buffer that holds block of type T elements.
template<typename T> class PipelineBuffer
{
    static const size_t bufferSize = _D_BUFFER_SIZE;
    T* bufferEnd;
    T storage[bufferSize];
public:
    T* begin() {return storage;}
    const T* begin() const {return storage;}
    T* end() const {return bufferEnd;}
    void setEnd( T* newPtr ) {bufferEnd=newPtr;}
    size_t maxSize() const {return bufferSize;}
    size_t size() const {return bufferEnd-begin();}
};

#endif

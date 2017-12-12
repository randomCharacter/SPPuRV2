#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defines.h"

// A concurrent hash table that maps strings to ints
typedef tbb::concurrent_hash_map<unsigned char, int> CounterTable;


// globals
extern tbb::concurrent_queue<unsigned char> in2HighPassQueue;
extern tbb::concurrent_queue<short> highPass2ClipQueue;
extern tbb::concurrent_vector<unsigned char> clip2CounterVector;
extern CounterTable counterValues;

#endif

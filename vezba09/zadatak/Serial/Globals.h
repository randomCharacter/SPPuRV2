#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include "defines.h"

// globals
extern std::queue<unsigned char> in2HighPassQueue;
extern std::queue<short> highPass2ClipQueue;
extern std::vector<unsigned char> clip2CounterVector;
extern unsigned int counterValues[256];

#endif

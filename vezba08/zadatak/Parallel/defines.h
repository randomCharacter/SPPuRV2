#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <iostream>
#include <fstream>
#include <string.h>
#include "tbb/concurrent_vector.h"
#include "tbb/concurrent_queue.h"
#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

#define _D_IN_FILE_NAME "./../infile.dat"
#define _D_OUT_FILE_NAME "./../outfileParallel.txt"
#define _D_ALPHA 0.5f
#define _D_CLIP_LOWER_VALUE 0
#define _D_CLIP_UPPER_VALUE 50
#define _D_ARRAY_SIZE 256
#define _D_GRAINSIZE 1000

typedef enum {RET_ERROR = -1, RET_OK = 0} RetVal;

#endif

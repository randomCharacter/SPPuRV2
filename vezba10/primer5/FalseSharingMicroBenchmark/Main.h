#pragma once

/**
* @brief Start appropriate memory test and measure total execution time.
* @param ucMallocType use __SCALABLE_ALLOCATOR__ for scalable allocator, 
					__SCALABLE_ALIGN_ALLOCATOR__ for alligned scalable alocator,
					or __REGULAR_ALLOCATOR__ for default allocator
			
* @param ucExampleType use __BUFFER__ for buffer example or
					__STL__ for STL vector example
*/
void test(unsigned char ucMallocType, unsigned char ucExampleType);


/**
* @brief Caclucate density on all threads results.
* @return density
*/
double calculateDensity();
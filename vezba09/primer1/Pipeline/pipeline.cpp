#include <stdio.h>

#include "tbb/tbb.h"

#include "MyBuffer.h"
#include "InputStage.h"
#include "MiddleStage.h"
#include "OutputStage.h"

using namespace tbb;


int main() {
	FILE *input_file, *output_file;

	input_file = fopen("../inputfile.txt", "r");
	output_file = fopen("../outputfile.txt", "w");

	// create the pipeline
	pipeline pipeline;

	// create file-reading stage and add it to the pipeline
	MyInputFilter input_filter(input_file);
	pipeline.add_filter(input_filter);

	// create capitalization stage and add it to the pipeline
	MyTransformFilter transform_filter;
	pipeline.add_filter(transform_filter);

	// create file-writing stage and add it to the pipeline
	MyOutputFilter output_filter(output_file);
	pipeline.add_filter(output_filter);

	// run the pipeline
	pipeline.run(MyInputFilter::n_buffer);

	// remove filters from pipeline before they are implicitly destroyed
	pipeline.clear();

	fclose(input_file);
	fclose(output_file);

	return 0;
}

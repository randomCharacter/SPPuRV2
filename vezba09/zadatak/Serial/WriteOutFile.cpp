#include "WriteOutFile.h"
#include "Globals.h"

using namespace std;

RetVal WriteOutFile(string fileName)
{
    ofstream outputFile(fileName.c_str());

	if (outputFile.is_open() == false)
	{
        cout << "WriteOutFile: Output file " << fileName << " could not be opened." << endl;
		return RET_ERROR;
	}


    for (int i=0; i<_D_ARRAY_SIZE; i++)
    {
        if (counterValues[i])
        {
            outputFile << i << ":\t" << counterValues[i] << endl;
        }
    }

    outputFile.close();

    return RET_OK;
}

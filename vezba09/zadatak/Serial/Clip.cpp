#include "Clip.h"
#include "Globals.h"

RetVal Clip(char lowerValue, char upperValue)
{
    short data;

    // clipping loop
    while (!highPass2ClipQueue.empty())
    {
        data = highPass2ClipQueue.front();
        highPass2ClipQueue.pop();

        if (data<lowerValue)
        {
            data = lowerValue;
        }
        else if (data>upperValue)
        {
            data = upperValue;
        }

        clip2CounterVector.push_back(data);
    }
    return RET_OK;
}

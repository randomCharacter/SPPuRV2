#include "Clip.h"
#include "Globals.h"

RetVal Clip(char lowerValue, char upperValue) {
    short data;

    // clipping loop
    while (!highPass2ClipQueue.empty()) {
        bool b = highPass2ClipQueue.try_pop(data);

        if (data<lowerValue) {
            data = lowerValue;
        } else if (data>upperValue) {
            data = upperValue;
        }

        clip2CounterVector.push_back(data);
    }
    return RET_OK;
}

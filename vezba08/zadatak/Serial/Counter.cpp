#include "Counter.h"
#include "Globals.h"

RetVal Counter() {
    unsigned char data;

    // count each value
    while (!clip2CounterVector.empty()) {
        data = clip2CounterVector.back();
        clip2CounterVector.pop_back();

        counterValues[data]++;
    }
    return RET_OK;
}

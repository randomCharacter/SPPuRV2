#include "HighPassFilter.h"
#include "Globals.h"

RetVal HighPassFilter(float alpha) {
    // input data
    static unsigned char xCurr = 0;
    static unsigned char xPrev = 0;

    // output data
    static unsigned char yCurr = 0;
    static unsigned char yPrev = 0;

    // filterring
    while (!in2HighPassQueue.empty()) {
        xPrev = xCurr;
        bool b = in2HighPassQueue.try_pop(xCurr);

        yPrev = yCurr;
        yCurr = alpha * (yPrev + xCurr - xPrev);

        highPass2ClipQueue.push(yCurr);
    }

    return RET_OK;
}

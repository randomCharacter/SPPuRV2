#include <iostream>
#include <stdlib.h>
#include "tbb/tick_count.h"

using namespace std;
using namespace tbb;

long SerialFib( long n );

int main(int argc, char* argv[])
{
    long n;
    long fibs;

    cout << endl << "Primer - fibonacijevi brojevi (serijski)" << endl;

    if(argc!=2)
    {
        cout << endl << "\tNiste uneli dovoljan broj argumenata";
        cout << endl << "\t\tPrimer: serialFib.exe 10" << endl;
        return 0;
    }

    n = atoi(argv[1]);

    tick_count startTime = tick_count::now();

    fibs = SerialFib(n);

    tick_count endTime = tick_count::now();

    cout << endl << n << ". fibonacijev broj je " << fibs << "." << endl;
    cout << endl << "Racunanje fibonacijevog broja je trajalo: " << (endTime-startTime).seconds()*1000 << "ms." << endl << endl;

    return 0;
}

long SerialFib( long n )
{
    if( n<2 )
    { 
        return n; 
    }
    else 
    {
        return SerialFib(n-1)+SerialFib(n-2); 
    }
}

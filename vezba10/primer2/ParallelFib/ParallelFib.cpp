#include <iostream>
#include <stdlib.h>
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include "tbb/task.h"

using namespace tbb;
using namespace std;

static long CutOff = 16;

long SerialFib( long n );
long ParallelFib( long n );

class FibTask: public task {
public:
    const long n;
    long* const sum;
    FibTask( long n_, long* sum_ ) :
        n(n_), sum(sum_)
    {}
    task* execute() {  // Overrides virtual function task::execute
        if( n<CutOff ) {
            *sum = SerialFib(n);
        } else {
            long x, y;
            FibTask& a = *new( allocate_child() ) FibTask(n-2,&x);
            FibTask& b = *new( allocate_child() ) FibTask(n-1,&y);
            // Set ref_count to "two children plus one for the wait".
            set_ref_count(3);
            // Start b running.
            spawn( b );
            // Start a running and wait for all children (a and b).
            spawn_and_wait_for_all(a);
            // Do the sum
            *sum = x+y;
        }
        return NULL;
    }
};

int main(int argc, char* argv[])
{

    long n;
    long fibs;

    cout << endl << "Primer - fibonacijevi brojevi (paralelizovan)" << endl;

    if(argc!=2)
    {
        cout << endl << "\tNiste uneli dovoljan broj argumenata";
        cout << endl << "\t\tPrimer: parallelFib.exe 10" << endl;
        return 0;
    }

    n = atoi(argv[1]);

    tick_count startTime = tick_count::now();

    // initialize tbb scheduler (required in TBB 2.1 and earlier
    task_scheduler_init init;

    fibs = ParallelFib(n);

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

long ParallelFib( long n )
{
    long sum;
    FibTask& a = *new(task::allocate_root()) FibTask(n,&sum);
    task::spawn_root_and_wait(a);
    return sum;
}

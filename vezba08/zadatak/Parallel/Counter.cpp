#include "Counter.h"
#include "Globals.h"


// Function object for counting occurrences of strings
struct Tally {
	CounterTable& table;
	Tally(CounterTable& table_) : table(table_) {}
	void operator()(const blocked_range<int> range) const {
		CounterTable::accessor a;
		for(int p=range.begin(); p!=range.end(); ++p) {
			table.insert(a, clip2CounterVector[p]);
			a->second += 1;
			a.release(); //the lock on object
		}
	}
};

RetVal Counter() {
  unsigned char data;

    // Put occurrences into the table
  	parallel_for (blocked_range<int>(0, clip2CounterVector.size(), 100), Tally(counterValues));

    return RET_OK;
}

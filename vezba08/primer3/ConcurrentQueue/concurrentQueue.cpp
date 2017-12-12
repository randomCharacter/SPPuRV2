#include "tbb/concurrent_queue.h"
#include <iostream>

using namespace tbb;
using namespace std;

// The iterators are intended only for debugging.  They are slow and not thread safe.
int main() {
	concurrent_queue<int> queue;
	for (int i=0; i<10; ++i) {
		queue.push(i);
	}
	for (concurrent_queue<int>::const_iterator i(queue.unsafe_begin()); i!= queue.unsafe_end(); ++i) {
		cout << *i << " ";
	}
	cout << endl;
	return 0;
}

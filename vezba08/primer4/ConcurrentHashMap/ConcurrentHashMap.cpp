#include <stdio.h>
#include <string>

#include "tbb/concurrent_hash_map.h"
#include "tbb/blocked_range.h"
#include "tbb/parallel_for.h"

using namespace std;
using namespace tbb;

// Structure that defines hashing and comparison operations for user's type
struct MyHashCompare {
	static size_t hash(const string& x) {
		size_t h = 0;
		for (const char* s = x.c_str(); *s; ++s) {
			h = (h*17)^*s;
		}
		return h;
	}

	// true if strings are equal
	static bool equal(const string& x, const string& y) {
		return x==y;
	}
};

// A concurrent hash table that maps strings to ints
typedef concurrent_hash_map<string, int, MyHashCompare> StringTable;

// Function object for counting occurrences of strings
struct Tally {
	StringTable& table;
	Tally(StringTable& table_) : table(table_) {}
	void operator()(const blocked_range<string*> range) const {
		StringTable::accessor a;
		for(string* p=range.begin(); p!=range.end(); ++p) {
			table.insert(a, *p);
			a->second += 1;
			a.release(); //the lock on object
		}
	}
};

const size_t N = 1000000;

string Data[N];

// Count occurrences
int main() {
	// Construct empty table
	StringTable table;

	Data[0] = "asd";
	Data[1] = "asda";
	Data[2] = "asdasd";
	Data[3] = "asd";
	Data[4] = "asd";

	// Put occurrences into the table
	parallel_for (blocked_range<string*>(Data, Data+N, 100), Tally(table));

	// Display the occurrences
	for (StringTable::iterator i = table.begin(); i!=table.end(); ++i) {
		printf("%s %d", i->first.c_str(), i->second);
	}

	return 0;
}

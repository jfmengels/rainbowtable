#include "Table.hpp"

using namespace std;

int Table::findIndex(const string& hash) const {
	// Searches for hash using the binary search algorithm.
	int min = 0, max = rows.size() - 1;
	int mid = (min + max) / 2, comp;
	while (min <= max) {
		comp = hash.compare(rows[mid].hash());
		if (comp == 0) {
			return mid;
		} else if (comp < 0) {
			max = mid - 1;
		} else {
			min = mid + 1;
		}
		mid = (min + max) / 2;
	}
	return min;
}
	
vector<string> Table::find(const string& hash) const {
	// Searches for hash using the binary search algorithm.
	// Returns all of the correspondings password.
	vector<string> results;
	int min = 0, max = rows.size() - 1;
	int mid = (min + max) / 2, comp;
	while (min <= max) {
		comp = hash.compare(rows[mid].hash());
		if (comp == 0) {
			// We found the corresponding hash, add it.
			results.insert(results.begin(), rows[mid].pwd());
			// Find all the other elements with key hash before mid, add them.
			min = mid - 1;
			while (min >= 0 &&
					hash.compare(rows[min].hash()) == 0) {
				results.insert(results.begin(), rows[min--].pwd());
			}
			// Find all the other elements with key hash after mid, add them.
			min = mid + 1;
			while (min < rows.size() &&
					hash.compare(rows[min].hash()) == 0) {
				results.insert(results.begin(), rows[min++].pwd());
			}
			return results;
		} else if (comp < 0) {
			max = mid - 1;
		} else {
			min = mid + 1;
		}
		mid = (min + max) / 2;
	}
	return results;
}
		
void Table::insert(const Pair& pair) {
	// Find the position where the pair should be inserted, and insert it.
	int index = findIndex(pair.hash());
	rows.insert(rows.begin()+index, pair);
}

void Table::insertAtEnd(const string& hash, const string& pwd) {
	Pair pair(pwd, hash);
	rows.push_back(pair);
}


ostream& Table::printTo(std::ostream& stream) const {
	vector<Pair>::const_iterator it;
	// For every hash-password pair, print it to the stream.
	for (it = rows.begin(); it < rows.end(); it++) {
		stream << (*it).pwd() << " " << (*it).hash() << endl;
	}
	return stream;
}

void Table::freeMemory() {
	rows.clear();
}



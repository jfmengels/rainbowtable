#include "Table.hpp"
#include <iostream>

/* TODO Remove IOSTREAM */

using namespace std;

int Table::findIndex(const string& hash) const {
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
	vector<string> results;
	int min = 0, max = rows.size() - 1;
	int mid = (min + max) / 2, comp;
	while (min <= max) {
		comp = hash.compare(rows[mid].hash());
		if (comp == 0) {
			// We found the corresponding hash.
			results.insert(results.begin(), rows[mid].pwd()); // Add the element
			// find all the other elements with key hash before mid
			min = mid - 1;
			while (min >= 0 &&
					hash.compare(rows[min].hash()) == 0) {
				results.insert(results.begin(), rows[min--].pwd());
			}
			// find all the other elements with key hash after mid
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
		
void Table::insert(const string& hash, const string& pwd) {
	Pair pair(pwd, hash);
	int index = findIndex(hash);
	rows.insert(rows.begin()+index, pair);
}

void Table::insertAtEnd(const string& hash, const string& pwd) {
	Pair pair(pwd, hash);
	rows.insert(rows.end(), pair);
}


ostream& Table::toString(std::ostream& stream) const {
	vector<Pair>::const_iterator it;
	for (it = rows.begin(); it < rows.end(); it++) {
		stream << (*it).pwd() << " " << (*it).hash() << endl;
	}
	return stream;
}

void Table::liberate() {
	rows.clear();
}



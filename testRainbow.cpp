#include <iostream>
#include <sys/time.h>
#include <iomanip>
#include "RainbowTable.hpp"
#include <signal.h>
#include <stdlib.h>

using namespace std;

void printInstructions() {
	cout << "help -- Displays this menu." << endl;
	cout << "new ncols nrows chars pwdlength hash --" << endl
		 << "\tCreates a new rainbow table with ncols columns, nrows rows," << endl
		 << "\tpasswords of length pwdLength, and using hash function hash" << endl
		 << "\t('md5' for md5 hash)." << endl;
	cout << "crackH hash -- Tries to find the password behind hash." << endl;
	cout << "crackW word -- Tries to find the password behing the hash of word." << endl;
	cout << "save file -- Saves a rainbow table in file." << endl;
	cout << "load file -- Load a rainbow table from file." << endl;
	cout << "quit -- Quits the program." << endl;
}

double computeTime(const struct timeval& t0) {
	struct timeval t1;
	gettimeofday(&t1, 0);
	double dTime1 = t0.tv_sec+(t0.tv_usec/1000000.0);
	double dTime2 = t1.tv_sec+(t1.tv_usec/1000000.0);
	return dTime2-dTime1;
}

void crackHash(const RainbowTable* rain, const string& hash) {
	struct timeval t;
	gettimeofday(&t, 0);	
	string res = rain->crackPassword(hash);
	double time = computeTime(t);
	
	cout << "'" << hash << "' --> ";
	if (res == "") {
		cout << "Not found...";
	} else {
		cout << "'" << res << "'";
	}
	cout << " (" << setprecision(4) << time << " seconds)" << endl;
}

void crackWord(const RainbowTable* rain, const string& pwd) {
	struct timeval t;
	gettimeofday(&t, 0);
	string res = rain->testWord(pwd);
	double time = computeTime(t);
	string hash = rain->hashWord(pwd);
	cout << "'" << pwd << "' --> '" << hash << "' --> ";
	if (res == "") {
		cout << "Not found...";
	} else {
		cout << "'" << res << "'";
	}
	cout << " (" << setprecision(4) << time << " seconds)"
		 << endl;
}

void newTable(RainbowTable** rain) {
	int ncols, nrows, pwdLength;
	string chars, hash;
	HashMethod* hashMethod;
	cin >> ncols;
	cin >> nrows;
	cin >> chars;
	cin >> pwdLength;
	cin >> hash;
	
	if (hash == "md5") {
		hashMethod = new MD5Hash();
	}
	struct timeval t;
	gettimeofday(&t, 0);
	*rain = new RainbowTable(ncols, nrows, chars, pwdLength, hashMethod);
	double time = computeTime(t);
	cout << "Table generated (" << setprecision(4)
		 << time << " seconds)" << endl;
}

void loadTable(RainbowTable** rain, const string& fileName) {
	delete *rain;
	struct timeval t;
	gettimeofday(&t, 0);
	*rain = new RainbowTable(fileName);
	double time = computeTime(t);
	cout << "Table loaded (" << setprecision(4)
		 << time << " seconds)" << endl;
}

void doAction(RainbowTable** rain, const string& action) {
	string param1;
	if (action == "help") {
		printInstructions();
	} else if (action == "new") {
		delete *rain;
		newTable(rain);
	} else if (action == "load") {
		cin >> param1;	// File name
		loadTable(rain, param1);
	} else if (action == "crackH") {
		cin >> param1;	// Hash to crack.
		crackHash(*rain, param1);
	} else if (action == "crackW") {
		cin >> param1;	// Word to hash, and then to crack.
		crackWord(*rain, param1);
	} else if (action == "save") {
		cin >> param1;	// File name
		(*rain)->writeToFile(param1);
	} else if (action != "quit") {
		cout << action << " is not a valid command." << endl;
		// Flush buffer
		cin.clear();
		fflush(stdin);
	}
}


RainbowTable* rain = NULL;

void sig_hnd(int sig) {
	delete rain;
	exit(EXIT_SUCCESS);
}


int main() {
	signal( SIGINT, sig_hnd );
	
	string action;
	cout << "Enter 'help' to learn the commands." << endl;
	cout << "You should first create or load a new rainbow table." << endl;
	while (action != "quit") {
		cout << "<<< ";
		cin >> action;
		doAction(&rain, action);
	}
	delete rain;
	return 0;
}

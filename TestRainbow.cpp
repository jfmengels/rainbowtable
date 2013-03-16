#include "TestRainbow.hpp"
#include <iostream>
#include <iomanip>
#include <signal.h>
#include <stdlib.h>

using namespace std;

RainbowTable** TestRainbow::_rainInstance = NULL;

TestRainbow::TestRainbow(bool isSilent)
{
	this->_isSilent = isSilent;
	_rain = NULL;
	TestRainbow::_rainInstance = &_rain;
	signal(SIGINT, TestRainbow::handleSignalCTRLC);
}

TestRainbow::~TestRainbow()
{
	delete _rain;
}

void TestRainbow::printInstructions() const
{
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

double TestRainbow::computeTime(const struct timeval& t0) const
{
	struct timeval t1;
	gettimeofday(&t1, 0);
	double dTime1 = t0.tv_sec+(t0.tv_usec/1000000.0);
	double dTime2 = t1.tv_sec+(t1.tv_usec/1000000.0);
	return dTime2-dTime1;
}

double TestRainbow::crackHash(const string& hash) const
{
	struct timeval t;
	gettimeofday(&t, 0);	
	string res = _rain->crackPassword(hash);
	double time = computeTime(t);
	
	cout << "'" << hash << "' --> ";
	if (res == "") {
		cout << "Not found...";
	} else {
		cout << "'" << res << "'";
	}
	cout << " (" << setprecision(4) << time << " seconds)" << endl;
	return time;
}

double TestRainbow::crackWord(const string& pwd) const
{
	struct timeval t;
	gettimeofday(&t, 0);
	string res = _rain->testWord(pwd);
	double time = computeTime(t);
	string hash = _rain->hashWord(pwd);
	cout << "'" << pwd << "' --> '" << hash << "' --> ";
	if (res == "") {
		cout << "Not found...";
	} else {
		cout << "'" << res << "'";
	}
	cout << " (" << setprecision(4) << time << " seconds)"
		 << endl;
	return time;
}

double TestRainbow::newTable()
{
	delete _rain;
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
	_rain = new RainbowTable(ncols, nrows, chars, pwdLength, hashMethod);
	double time = computeTime(t);
	cout << "Table generated (" << setprecision(4)
		 << time << " seconds)" << endl;
	 return time;
}

double TestRainbow::loadTable(const string& fileName)
{
	delete _rain;
	struct timeval t;
	gettimeofday(&t, 0);
	_rain = new RainbowTable(fileName);
	double time = computeTime(t);
	cout << "Table loaded (" << setprecision(4)
		 << time << " seconds)" << endl;
	return time;
}

void TestRainbow::doAction(const string& action)
{
	string param1;
	if (action == "help") {
		printInstructions();
	}
	else if (action == "new") {
		newTable();
	}
	else if (action == "load") {
		cin >> param1;	// File name
		loadTable(param1);
	}
	else if (_rain == NULL && action != "quit") {
		cout << "***You need to create or load a table first." << endl;
	}
	else if (action == "crackH") {
		cin >> param1;	// Hash to crack.
		crackHash(param1);
	}
	else if (action == "crackW") {
		cin >> param1;	// Word to hash, and then to crack.
		crackWord(param1);
	}
	else if (action == "save") {
		cin >> param1;	// File name
		_rain->writeToFile(param1);
	}
	else if (action != "quit") {
		cout << action << " is not a valid command." << endl;
		// Flush buffer
		cin.clear();
		fflush(stdin);
	}
}

void TestRainbow::handleSignalCTRLC(int signal)
{
	delete *(TestRainbow::_rainInstance);
	cout << endl;
	exit(EXIT_SUCCESS);
}


int main() {	
	TestRainbow test;
	string action;
	cout << "Enter 'help' to learn the commands." << endl;
	cout << "You should first create or load a new rainbow table." << endl;
	while (action != "quit") {
		cout << ">>> ";
		cin >> action;
		test.doAction(action);
	}
	return 0;
}

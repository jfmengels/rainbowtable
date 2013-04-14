#include "RainbowTable.hpp"	/* Rainbow table */
#include <time.h>		/* For randomness, via srand() */
#include <stdlib.h>		
#include <iostream>		/* cout */
#include <cstring>		/* str.c_str() */
#include <fstream>		/* File input / output */
#include <omp.h>		/* Parallelism with OpenMP */
#include "Table.hpp"	/* Pair class */

using namespace std;

RainbowTable::RainbowTable(std::string const& fileName)
{
	this->readFromFile(fileName);
}

RainbowTable::RainbowTable(int columns, int rows, string chars,
		int pwdLength, HashMethod* hashMethod=NULL)
{
	this->columns = columns;
	this->rows = rows;
	this->chars = chars;
	this->pwdLength = pwdLength;
	this->hashMethod = hashMethod;
	replace(chars, "a-z", LETTERSLOWER);
	replace(chars, "A-Z", LETTERSUPPER);
	replace(chars, "0-9", DIGITS);
	this->initTable();
}

void RainbowTable::initTable()
{
	// While generating the contents of the table (chains), every thread will
	// add the pairs it generated to the table. To avoid concurrency, this will
	// only be done when the lock isn't set.
	omp_lock_t lock;
	omp_init_lock(&lock);

	#pragma omp parallel // Parallelizes the generation. Every thread will
	{					 // generate <rows / nb of threads> rows.
		string pwd, hash;
		vector<Pair> v;
		int nProc = omp_get_thread_num(); // Get thread number
		int totalProc = omp_get_num_threads(); // Get total number of threads

		for (int i = nProc; i < rows; i+= totalProc) {
			pwd = this->randomPassword();	// Generate a new password
			// Generate a chain, and retrieve its last hash.
			hash = this->createChain(pwd);

			// Add the pair password - hash to a temporary vector.
			Pair pair(pwd, hash);
			v.push_back(pair);

			// Add the pairs in the temporary vector in the table, if no other
			// thread is currently doing the same thing. The vector will be
			// empty at the end.
			if (omp_test_lock(&lock)) {
				while (!v.empty()) {
					table.insert(v.back());
					v.pop_back();
				}
				omp_unset_lock(&lock);
			}
		}
		// Add the remaining pairs in the temporary vector in the table.
		omp_set_lock(&lock);
		while (!v.empty()) {
			table.insert(v.back());
			v.pop_back();
		}
		omp_unset_lock(&lock);
	}
	omp_destroy_lock(&lock);
}

RainbowTable::~RainbowTable()
{
	delete hashMethod;
	table.freeMemory();
}

void RainbowTable::writeToFile(std::string const& fileName) const
{
	ofstream out(fileName.c_str());
	if (out) {
		// Write the parameters of the table.
		out << this->columns << " "	// Length of the chains.
			<< this->chars << " "	// Chars covered
			<< this->pwdLength << " " // Length of the passwords covered
			<< this->hashMethod->name() << endl; // Name of the hashing method.
		// Write the chains.
		table.printTo(out);
	} else {
		cerr << "Could not write to file \"" << fileName << "\"." << endl;
	}
	
	out.close();
}

void RainbowTable::readFromFile(std::string const& fileName)
{
	ifstream in(fileName.c_str());
	if (in) {
		// Read the parameters from the file.
		string hashMethodName;
		in >> this->columns;	// Length of the chains.
		in >> this->chars;		// Chars covered
		in >> this->pwdLength;	// Length of the passwords covered
		in >> hashMethodName;	// Name of the hashing method
		
		// Recover hashing method from its name.
		if (hashMethodName == "md5") {
			this->hashMethod = new MD5Hash();
		}

		// Read the chains.
		string pwd, hash;
		in >> pwd;
		in >> hash;
		while(in) { // While file contains rows, read and insert
					// them in the table.
			this->table.insertAtEnd(hash, pwd);
			in >> pwd;
			in >> hash;
		}
	} else {
		cerr << "Could not read from file \"" << fileName << "\"." << endl;
	}
}

std::string RainbowTable::hashWord(std::string const& pwd) const
{
	// Hashes a word using the table's hashing method.
	return this->hashMethod->hash(pwd);
}

std::string RainbowTable::randomPassword() const
{
	// Generates a new password.
	std::string pwd;
	// Get <pwdLength> random characters from the covered characters, and
	// concatenate& them. 
	for (int i=0; i < this->pwdLength; i++) {
		pwd += this->chars[rand() % this->chars.size()];
	}
	return pwd;
}

void hexconvert(const char *text, unsigned char* bytes)
{
    int temp;
	// WARNING: Current implementation specific to MD5.
	// Transform the hash (string) into a byte array.
	// Since every byte is exressed with 2 chars, read it 2 by 2,
	// convert it to an int and store it into the bytes array.
    for(int i = 0; i < 16; i++) {
        sscanf( text + 2 * i, "%2x", &temp );
        bytes[i] = temp;
    }
}

std::string RainbowTable::reduce(const std::string& hash, int column) const
{
	string pwd;
	int index;
	// WARNING : Current implementation specific to MD5.
	unsigned char* bytes = new unsigned char[16];
	// Transform the hash into an array of bytes.
	hexconvert(hash.c_str(), bytes);
	for (int i = 0; i < pwdLength; i++) {
		// Get the value of the i-nd byte of the hash. The value column is
		// added so as to same inputs at different columns will generate a
		// different reduced password. This will avoid too many repetitions
		// in the table.
		index = bytes[(i+column) % 16];
		// Get the corresponding character and concatenate it to the password.
		pwd += this->chars[index % this->chars.size()];
	}
	delete[] bytes;
	return pwd;
}

std::string RainbowTable::createChain(std::string pwd) const
{
	string hash;
	// Hash and reduce the starting password <columns> times.
	for (int col=0; col < this->columns; col++) {
		hash = this->hashWord(pwd);
		pwd = this->reduce(hash, col);
	}
	// Return the final hash.
	return hash;
}

std::string RainbowTable::crackPassword(std::string const& startHash) const
{

	string result;
	#pragma omp parallel // Parallelizes the cracking. Every thread will
	// try cracking for < columns / nb of threads > different columns.
	{
		int nProc = omp_get_thread_num(); // Get thread number
		int totalProc = omp_get_num_threads(); // Get total number of threads
		
		string hash, pwd;
		vector<string> possiblePwd;
		vector<string>::const_iterator it;
		
		for (int col=this->columns-1-nProc; col>=0 && result == ""; col-=totalProc) {
			// Compute the final hash, when starting at column <col>.
			hash = this->getFinalHash(startHash, col);
			// Find the start passwords corresponding to the hash (possibly 0, 1 or more).
			possiblePwd = this->table.find(hash);
			for (it=possiblePwd.begin(); it < possiblePwd.end(); it++) {
				// For every start password, try to find if the hash is contained in it.
				pwd = this->findHashInChain(*it, startHash);
				if (pwd != "") {
					// If the hash has been found, store the corresponding
					// password, causing the loop to stop.
					result = pwd;
				}
			}
		}
	}
	return result;
}

std::string RainbowTable::getFinalHash(const std::string& startHash,
			int startCol) const
{
	string hash = startHash;
	string pwd;
	// Hash and reduce the starting password <columns-starCol> times.
	for (int col=startCol; col < this->columns-1; col++) {
		pwd = this->reduce(hash, col);
		hash = this->hashWord(pwd);
	}
	return hash;
}

std::string RainbowTable::findHashInChain(const std::string& startPwd,
			const std::string& startHash) const
{
	int col = 0;
	string hash = this->hashWord(startPwd);
	string pwd;
	// Hash and reduce the password until the end of the chain has been reached,
	// or startHash has been found.
	if (hash == startHash) {
		return startPwd;
	}
	while (col < this->columns) {
		pwd = this->reduce(hash, col);
		hash = this->hashWord(pwd);
		if (hash == startHash) {
			return pwd;
		}
		col++;
	}
	return "";
}

std::string RainbowTable::testWord(const std::string& word) const
{
	// Hashes a password, then tries to crack it.
	string hash = this->hashWord(word);
	return this->crackPassword(hash);
}

void RainbowTable::replace(std::string& str, const std::string& from, const std::string& to) {
	// Find string <from>.
    size_t start_pos = str.find(from);
    if(start_pos != std::string::npos) { // If found, replace sequence to <to>.
    	str.replace(start_pos, from.length(), to);
	}
}

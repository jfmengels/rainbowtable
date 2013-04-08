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
	
	this->initTable();
}

void RainbowTable::initTable()
{
	omp_lock_t lock;
	omp_init_lock(&lock);

	#pragma omp parallel
	{
		string pwd, hash;
		vector<Pair> v;
		int nProc = omp_get_thread_num();
		int totalProc = omp_get_num_threads();
		for (int i = nProc; i < rows; i+= totalProc) {
			pwd = this->randomPassword();
			hash = this->createChain(pwd);
			Pair pair(pwd, hash);
			v.push_back(pair);
			if (omp_test_lock(&lock)) {
				while (!v.empty()) {
					table.insert(v.back());
					v.pop_back();
				}
				omp_unset_lock(&lock);
			}
		}
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
		out << this->columns << " "
			<< this->chars << " "
			<< this->pwdLength << " "
			<< this->hashMethod->name() << endl;
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
		string hashMethodName;
		in >> this->columns;
		in >> this->chars;
		in >> this->pwdLength;
		in >> hashMethodName;
		
		if (hashMethodName == "md5") {
			this->hashMethod = new MD5Hash();
		}
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
	return this->hashMethod->hash(pwd);
}

std::string RainbowTable::randomPassword() const
{
	std::string pwd;
	for (int i=0; i < this->pwdLength; i++) {
		pwd += this->chars[rand() % this->chars.size()];
	}
	return pwd;
}

void hexconvert(const char *text, unsigned char* bytes)
{
    int temp;
    for(int i = 0; i < 16; i++) {
        sscanf( text + 2 * i, "%2x", &temp );
        bytes[i] = temp;
    }
}

std::string RainbowTable::reduce(const std::string& hash, int column) const
{
	string pwd;
	int index;
	unsigned char* bytes = new unsigned char[16];
	hexconvert(hash.c_str(), bytes);
	for (int i = 0; i < pwdLength; i++) {
		index = bytes[(i+column) % 16];
		if (index < 0) {
			index = -index;
		}
		pwd += this->chars[index % this->chars.size()];
	}
	delete[] bytes;
	return pwd;
}

std::string RainbowTable::createChain(std::string pwd) const
{
	string hash;
	for (int col=0; col < this->columns; col++) {
		hash = this->hashWord(pwd);
		pwd = this->reduce(hash, col);
	}
	return hash;
}

std::string RainbowTable::crackPassword(std::string const& startHash) const
{

	string result;
	#pragma omp parallel
	{
		int nProc = omp_get_thread_num();
		int totalProc = omp_get_num_threads();
		
		string hash, pwd;
		vector<string> possiblePwd;
		vector<string>::const_iterator it;
		
		for (int col=this->columns-1-nProc; col>=0 && result == ""; col-=totalProc) {
			hash = this->getFinalHash(startHash, col);
			possiblePwd = this->table.find(hash);
			for (it=possiblePwd.begin(); it < possiblePwd.end(); it++) {
				pwd = this->findHashInChain(*it, startHash);
				if (pwd != "") {
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
	string hash = this->hashWord(word);
	return this->crackPassword(hash);
}



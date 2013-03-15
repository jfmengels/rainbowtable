#ifndef RAINBOW_HPP
#define RAINBOW_HPP
#include <string>
#include <vector>
#include "HashMethod.hpp"
#include "Table.hpp"
#define LETTERSLOWER std::string("abcdefghijklmnopqrstuvwxyz")
#define LETTERSUPPER std::string("ABCDEFGHIJKLMNOPQRSTUVWXYZ")
#define DIGITS std::string("0123456789")
#define ALL LETTERSLOWER+LETTERSUPPER+DIGITS

class RainbowTable;
class HashMethod;
class Table;
class Pair;


class RainbowTable {
	private:
		int columns;		/* Number of columns for each chain. */
		int rows;			/* Number of rows too generate */
		std::string chars;		/* Array of characters to check for. */
		int pwdLength;		/* Size of the passwords to check. */
		Table table;		/* table containing all the rows (hash + password) */
		HashMethod* hashMethod; /* hashing function */
		
		void fillTable();
		std::vector<std::string> findHash() const;
		std::string reduce(const std::string& hash, int column) const;
		std::string createChain(std::string pwd) const;
		std::string randomPassword() const;
		std::string getFinalHash(const std::string& startHash,
					int startCol) const;
		std::string findHashInChain(const std::string& startPwd,
					const std::string& startHash) const;
		
	
	public:
		RainbowTable(std::string const& fileName);
		RainbowTable(int columns, int rows, std::string chars, int pwdLength, HashMethod* hashMethod);
		~RainbowTable();
		void writeToFile(const std::string& fileName) const;
		void readFromFile(const std::string& fileName);
		std::string hashWord(const std::string& pwd) const;
		std::string crackPassword(const std::string& startHash) const;
		std::vector<std::string> allPasswords() const;
		
		std::string testWord(const std::string& word) const;
		void printTable() const;
		
		
};

#endif

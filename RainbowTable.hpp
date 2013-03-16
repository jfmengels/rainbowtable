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

class RainbowTable
{
	private:
		int columns;		/* Number of columns for each chain. */
		int rows;			/* Number of rows too generate */
		std::string chars;		/* Array of characters to check for. */
		int pwdLength;		/* Size of the passwords to check. */
		Table table;		/* table containing all the rows (hash + password) */
		HashMethod* hashMethod; /* Hashing function */
		
		/**
		  * Initializes the table, by generating its content.
		  */
		void initTable();
		
		/**
		  * Reduces a hash.
		  * hash : Hash to reduce.
		  * column : Index of the column at which to reduce.
		  * Returns a new correct password.
		  */
		std::string reduce(const std::string& hash, int column) const;
		
		/**
		  * Creates the hash corresponding to the end of a chain, starting from
		  * pwd.
		  * pwd : Password to start the chain from.
		  * Returns the hash at the end of the table.
		  */
		std::string createChain(std::string pwd) const;
		
		/**
		  * Generates and returns a new correct password.
		  */
		std::string randomPassword() const;
		
		/**
		  * Returns the hash corresponding to the end of a chain.
		  * startHash : Hash to start from.
		  * startCol : Index of the column to start the reducing/hashing from.
		  */
		std::string getFinalHash(const std::string& startHash,
					int startCol) const;
					
		/**
		  * Finds a hash in a chain.
		  * startPwd : Start password of the chain.
		  * startHash : Hash to find.
		  * Returns the password associated to the hash if it is found,
		  *			"" otherwise.
		  */
		std::string findHashInChain(const std::string& startPwd,
					const std::string& startHash) const;
		
	
	public:
		/**
		  * Creates a new table, which will be loaded from a file.
		  * fileName : Name of the file to read from.
		  */
		RainbowTable(std::string const& fileName);
		
		/**
		  * Creates and generates a new table.
		  * columns : Number of columns, i.e. the length of each chain.
		  * rows : Number of rows to generate.
		  * chars : String containing all the characters available to make a
		  *			password from. Example: abcdefghijklmnopqrstuvwxyz
		  * pwdLength : Length of the password to cover.
		  * hashMethod : Hashing method.
		  */
		RainbowTable(int columns, int rows, std::string chars, int pwdLength, HashMethod* hashMethod);
		
		/**
		  * Destroys the table, freeing the memory.
		  */
		~RainbowTable();
		
		/**
		  * Write the table to a file.
		  * fileName : Name of the file to write to.
		  */
		void writeToFile(const std::string& fileName) const;
		
		/**
		  * Read a table from a file.
		  * fileName : Name of the file to read from.
		  */
		void readFromFile(const std::string& fileName);
		
		/**
		  * Hash a password.
		  * pwd : Password to hash.
		  * Returns the hash corresponding to the password.
		  */
		std::string hashWord(const std::string& pwd) const;
		
		/**
		  * Tries to crack a hash.
		  * startHash : Hash to crack.
		  * Returns the associated password if found, "" otherwise.
		  */
		std::string crackPassword(const std::string& startHash) const;
		
		/**
		  * Hashes a password and tries to crack it.
		  * word : Password to hash, and then to crack.
		  * Returns word if the password is found, "" otherwise.
		  */
		std::string testWord(const std::string& word) const;

};

#endif

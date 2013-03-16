#ifndef TABLE_HPP
#define TABLE_HPP
#include <vector>
#include <string>
#include <fstream>


class Pair; /* Defined below the definition of Table. */

/**
  * Structure containing all the hash-password pairs.
  */
class Table {
	private:
		std::vector<Pair> rows; /* Container for the pairs. */
		
		/**
		  * Finds the index at which hash should be inserted (table is ordered).
		  * Does not do the actual insertion.
		  * hash : hash to insert.
		  */
		int findIndex(const std::string& hash) const;
	
	public:
		/**
		  * Finds a hash in the table.
		  * hash : Hash to find.
		  * Returns the corresponding password if found, "" otherwise.
		  */
		std::vector<std::string> find(const std::string& hash) const;
		
		/**
		  * Inserts a pair in the table. Insertion is done at the appropriate
		  * position, and in O(log n) time.
		  * pair : Pair to insert.
		  */	
		void insert(const Pair& pair);
		
		/**
		  * Inserts a pair at the end of the table. Is used when loading from a
		  * file. Insertion is done in constant amortized time.
		  * hash - pwd : pair to insert.
		  */
		void insertAtEnd(const std::string& hash, const std::string& pwd);
		
		/**
		  * Prints the content of the table to stream.
		  * stream : Output stream to write to.
		  */
		std::ostream& printTo(std::ostream& stream) const;
		
		/**
		  * Frees the table memory.
		  */
		void freeMemory();
};


/**
  * Pair of hash (key) and password (value).
  */
class Pair {
	private:
		std::string _pwd;
		std::string _hash;
	public:
		Pair(const std::string& p_pwd, const std::string& p_hash) {
			_pwd = p_pwd;
			_hash = p_hash;
		}
		std::string pwd() const {
			return _pwd;
		}
		std::string hash() const {
			return _hash;
		}
};
#endif

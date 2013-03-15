#ifndef TABLE_HPP
#define TABLE_HPP
#include <vector>
#include <string>
#include <fstream>

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

class Table {
	private:
		std::vector<Pair> rows;
		int findIndex(const std::string& hash) const;
	
	public:
		std::vector<std::string> find(const std::string& hash) const;	
		void insert(const Pair& pair);
		void insertAtEnd(const std::string& hash, const std::string& pwd);
		std::ostream& toString(std::ostream& stream) const;
		void liberate();
	
};
#endif

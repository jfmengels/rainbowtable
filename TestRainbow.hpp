#ifndef TESTRAINBOW_HPP
#define TESTRAINBOW_HPP

#include "RainbowTable.hpp"
#include <sys/time.h>
#include <string>

class TestRainbow
{
	public:
		/**
		  * Creates the command line interface.
		  * isSilent : false if commands should display results when invoked,
		  *			   true otherwise.
		  */
		TestRainbow(bool isSilent=false);
		
		/**
		  * Destroys the interface, freeing up the RainbowTable it contains.
		  */
		~TestRainbow();
		
		/**
		  * Executes a command specified by action.
		  * action : command name catched from user input.
		  */
		void doAction(const std::string& action);
		
	private:
		/***************** Atrributes *****************/
		/* Rainbow table */
		RainbowTable* _rain;
		/* isSilent : false if commands should display results when invoked,
		true otherwise. */
		bool _isSilent;
		/* Static pointer to _rain. Used so that static method handleSignalCTRLC
		can free memory when user interrupts the execution. */
		static RainbowTable** _rainInstance;
	
	
		/***************** Methods *****************/
		/**
		  * Prints the commands available to the user.
		  */
		void printInstructions() const;

		/**
		  * Computes the time since t0, and returns it as seconds.
		  * t0 : Start time.
		  */
		double computeTime(const struct timeval& t0) const;

		/**
		  * Cracks a hash.
		  * hash : Hash to crack.
		  * Returns the time the operation took, and hasFound will equal true
		  * if the operation was a success, false otherwise.
		  */
		double crackHash(const std::string& hash, bool& hasFound) const;

		/**
		  * Cracks the hash associated to a password.
		  * pwd : Password to hash, and then to crack.
		  * Returns the time the operation took, and hasFound will equal true
		  * if the operation was a success, false otherwise.
		  */
		double crackWord(const std::string& pwd, bool& hasFound) const;
		
		/**
		  * Creates a new table, using the user inputted arguments.
		  * Returns the time the operation took.
		  */
		double newTable();

		/**
		  * Loads a table from a file.
		  * fileName : Name of the file to read in.
		  * Returns the time the operation took.
		  */
		double loadTable(const std::string& fileName);

		/**
		  * Generates a file containing valid random passwords.
		  * n : Number of passwords to generate.
		  * fileName : Name of the file to write to.
		  * Returns the time the operation took.
		  */
		double generatePwdFile(int n, const std::string& fileName);

		/**
		  * Reads a list of passwords from a file, and tries to crack them.
		  * fileName : Name of the file to write to.
		  * Returns the success percentage.
		  */
		double testPwdFile(const std::string& fileName);

		/**
		  * Handles the CTRL-C (interruption) signal.
		  */
		static void handleSignalCTRLC(int signal);

};

#endif

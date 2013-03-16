#ifndef TESTRAINBOW_HPP
#define TESTRAINBOW_HPP

#include "RainbowTable.hpp"
#include <sys/time.h>
#include <string>

class TestRainbow
{
	public:
		TestRainbow(bool isSilent=false);
		~TestRainbow();
		void doAction(const std::string& action);
		
	private:
		RainbowTable* _rain;
		bool _isSilent;
		static RainbowTable** _rainInstance;
	
	
		void printInstructions() const;

		double computeTime(const struct timeval& t0) const;

		double crackHash(const std::string& hash) const;

		double crackWord(const std::string& pwd) const;
		double newTable();

		double loadTable(const std::string& fileName);

		static void handleSignalCTRLC(int signal);

};

#endif

#ifndef COMMAND_H_
#define COMMAND_H_

#include <string>
using namespace std;

class Command {
private:
	string commandWord;
	string secondWord;

public:
	Command(string firstWord, string secondWord);
	string getCommandWord();
	string getSecondWord();
	bool isUnknown();
	bool hasSecondWord();
};

#endif /*COMMAND_H_*/

#ifndef RUNNER_H_
#define RUNNER_H_

#include "ParameterParser.h"

/**
 * Contains classes for the command line running mode of the application
 */
namespace command {

/**
 * Runs the application in command line mode
 */
class Runner {
private:
	/**
	 * The parser for the user input parameters
	 */
	ParameterParser parser;
public:
	/**
	 * Constructs a Runner
	 * @param argc The number of input parameters
	 * @param argv The input parameter array
	 */
	Runner(int argc, char** argv);
	/**
	 * Starts the application
	 */
	void run();
	virtual ~Runner();
};

}

#endif /* RUNNER_H_ */

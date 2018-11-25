#ifndef LOGGER_H_
#define LOGGER_H_

#include <string>
#include <sstream>

using namespace std;

class Logger {
private:
	ofstream* file;
	static Logger* instance;
	Logger(string);
	string filename;
public:
	template <class T> void log(T value) {
		stringstream ss;
		ss << value;
		log(ss.str());
	}
	void log(string);
	void logError(string);
	static Logger *getInstance();
	~Logger();
};

#endif /* LOGGER_H_ */

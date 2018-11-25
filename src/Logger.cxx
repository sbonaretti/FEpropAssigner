#include "Logger.h"

#include <fstream>
#include <iostream>
#include <cstdio>
#include <time.h>

using namespace std;

Logger* Logger::instance = new Logger("log.txt");

Logger::Logger(string filename) {
	//remove(filename.c_str());
	this->filename = filename;
	this->file = new ofstream(filename.c_str(), ios::out);
}

void Logger::log(string text) {
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	string stime = asctime(timeinfo);

	text = stime.append(text);
	file->write(text.c_str(), text.length());
}

void Logger::logError(string text) {
	this->log("Error: " + text);
	this->log("Abort program execution");
	exit(1);
}

Logger::~Logger() {
	file->close();
	delete [] file;
}

Logger* Logger::getInstance() {
	return Logger::instance;
}

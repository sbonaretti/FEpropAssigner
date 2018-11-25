#ifndef PROPERTYWRITER_H_
#define PROPERTYWRITER_H_

#include <string>

namespace mesh {

using namespace std;

/**
 * Abstract writer class to write a mesh file with property assignment information to disk
 */
class PropertyWriter {
protected:
	/**
	 * The file string
	 */
	string file;
public:
	/**
	 * Creates a new writer
	 * @param file The file string representing a file on disk
	 */
	PropertyWriter(string file);
	/**
	 * Writes the file
	 */
	virtual void write() = 0;
};

}

#endif /* PROPERTYWRITER_H_ */

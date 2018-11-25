#ifndef ELEMENTPROPERTYWRITER_H_
#define ELEMENTPROPERTYWRITER_H_

#include "ElementPropertyAssigner.h"
#include "PropertyWriter.h"
#include <string>

namespace mesh {

using namespace assignment;

/**
 * Abstract writer class to write a mesh file with element property assignment information to disk
 */
class ElementPropertyWriter : public PropertyWriter {
protected:
	/**
	 * The assigner that was used for the assignment
	 */
	ElementPropertyAssigner* assigner;
public:
	/**
	 * Creates a new writer
	 * @param file The file string
	 * @param assigner The assignment::ElementPropertyAssigner
	 */
	ElementPropertyWriter(string file, ElementPropertyAssigner* assigner);
};

}

#endif /* ELEMENTPROPERTYWRITER_H_ */

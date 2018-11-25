#ifndef ABAQUSELEMENTPROPERTYWRITER_H_
#define ABAQUSELEMENTPROPERTYWRITER_H_

#include <string>
#include "ElementPropertyWriter.h"
#include "ElementPropertyAssigner.h"

namespace mesh {

/**
 * Writes a mesh file with element property assignment information to disk in Abaqus format
 */
class AbaqusElementPropertyWriter : public ElementPropertyWriter {
public:
	/**
	 * Creates a new writer
	 * @param file The file string
	 * @param assigner The assignment::ElementPropertyAssigner
	 */
	AbaqusElementPropertyWriter(string file, assignment::ElementPropertyAssigner* assigner);
	void write();
};

}

#endif /* ABAQUSELEMENTPROPERTYWRITER_H_ */

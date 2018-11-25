#ifndef ANSYSELEMENTPROPERTYWRITER_H_
#define ANSYSELEMENTPROPERTYWRITER_H_

#include "ElementPropertyWriter.h"
#include <string>

namespace mesh {

/**
 * Writes a mesh file with element property assignment information to disk in Ansys format
 */
class AnsysElementPropertyWriter : public ElementPropertyWriter {
public:
	/**
	 * Creates a new writer
	 * @param file The file string
	 * @param assigner The assignment::ElementPropertyAssigner
	 */
	AnsysElementPropertyWriter(string file, assignment::ElementPropertyAssigner* assigner);
	void write();
};

}

#endif /* ANSYSELEMENTPROPERTYWRITER_H_ */

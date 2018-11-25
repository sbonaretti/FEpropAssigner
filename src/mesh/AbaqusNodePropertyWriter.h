#ifndef ABAQUSNODEPROPERTYWRITER_H_
#define ABAQUSNODEPROPERTYWRITER_H_

#include "NodePropertyAssigner.h"
#include "NodePropertyWriter.h"

namespace mesh {

using namespace assignment;

/**
 * Writes a mesh file with node property assignment information to disk in Abaqus format
 */
class AbaqusNodePropertyWriter : public NodePropertyWriter {
public:
	/**
	 * Creates a new writer
	 * @param file The file string
	 * @param assigner The assignment::NodePropertyAssigner
	 */
	AbaqusNodePropertyWriter(string file, NodePropertyAssigner* assigner);
	void write();
};

}

#endif /* ABAQUSNODEPROPERTYWRITER_H_ */


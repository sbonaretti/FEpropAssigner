#ifndef ANSYSNODEPROPERTYWRITER_H_
#define ANSYSNODEPROPERTYWRITER_H_

#include "NodePropertyAssigner.h"
#include "NodePropertyWriter.h"

namespace mesh {

using namespace assignment;

/**
 * Writes a mesh file with node property assignment information to disk in Ansys format
 */
class AnsysNodePropertyWriter : public NodePropertyWriter {
public:
	/**
	 * Creates a new writer
	 * @param file The file string
	 * @param assigner The assignment::NodePropertyAssigner
	 */
	AnsysNodePropertyWriter(string file, NodePropertyAssigner* assigner);
	void write();
};

}

#endif /* ANSYSNODEPROPERTYWRITER_H_ */


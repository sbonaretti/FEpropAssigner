#ifndef NODEPROPERTYWRITER_H_
#define NODEPROPERTYWRITER_H_

#include <string>
#include "PropertyWriter.h"
#include "NodePropertyAssigner.h"

namespace mesh {
using namespace assignment;

/**
 * Abstract writer class to write a mesh file with node property assignment information to disk
 */
class NodePropertyWriter : public PropertyWriter {
protected:
	/**
	 * The assigner that was used for the assignment
	 */
	NodePropertyAssigner* assigner;
public:
	/**
	 * Creates a new writer
	 * @param file The file string
	 * @param assigner The assignment::NodePropertyAssigner
	 */
	NodePropertyWriter(string file, NodePropertyAssigner* assigner);
};

}

#endif /* NODEPROPERTYWRITER_H_ */

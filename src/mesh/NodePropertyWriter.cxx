#include "NodePropertyWriter.h"

namespace mesh {

NodePropertyWriter::NodePropertyWriter(string file, NodePropertyAssigner* assigner) : PropertyWriter(file) {
	this->assigner = assigner;
}

}

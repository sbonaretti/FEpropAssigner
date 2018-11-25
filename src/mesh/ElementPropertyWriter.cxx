#include "ElementPropertyWriter.h"

namespace mesh {

ElementPropertyWriter::ElementPropertyWriter(string file, ElementPropertyAssigner* assigner) : PropertyWriter(file) {
	this->assigner = assigner;
}
}

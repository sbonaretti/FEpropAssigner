#include "Mesh.h"

namespace mesh {

Mesh::~Mesh() {

}

vector<Mesh::Triangle>* Mesh::getTriangleVector() {
	return &this->triangleVector;
}

vector<Mesh::Point>* Mesh::getPointVector() {
	return &this->pointVector;
}

vector<int>* Mesh::getNodeIds() {
	return &this->nodeIds;
}

map<int, Mesh::Element>* Mesh::getElements() {
	return &this->elements;
}

}

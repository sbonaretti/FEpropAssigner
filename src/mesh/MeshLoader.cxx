#include "MeshLoader.h"

namespace mesh {

using namespace std;

QSharedPointer<Mesh> MeshLoader::getMesh() {
	return this->mesh;
}

MeshLoader::~MeshLoader() {

}

}

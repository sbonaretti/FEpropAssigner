#ifndef MESHLOADER_H_
#define MESHLOADER_H_

#include "Mesh.h"

#include <memory>
#include <QSharedPointer>

namespace mesh {

using namespace std;

/**
 * Loads a mesh from disk
 */
class MeshLoader {
protected:
	/**
	 * The laoded mesh::Mesh
	 */
	QSharedPointer<Mesh> mesh;
public:
	/**
	 * Gets the loaded mesh::Mesh
	 * @return The loaded mesh::Mesh
	 */
	QSharedPointer<Mesh> getMesh();
	virtual ~MeshLoader();
};

}

#endif /* MESHLOADER_H_ */

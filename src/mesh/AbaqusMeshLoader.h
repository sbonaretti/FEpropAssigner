#ifndef ABAQUSMESHLOADER_H_
#define ABAQUSMESHLOADER_H_

#include "MeshLoader.h"

namespace mesh {

/**
 * Loads an Abaqus mesh file from disk
 */
class AbaqusMeshLoader: public MeshLoader {
public:
	/**
	 * Creates a new loader and reads the specified Abaqus file
	 * @param file The Abaqus file
	 */
	AbaqusMeshLoader(string file);
	virtual ~AbaqusMeshLoader();
private:
	void findSurfaceNodes();
};

}

#endif /* ABAQUSMESHLOADER_H_ */

#ifndef ANSYSMESHLOADER_H_
#define ANSYSMESHLOADER_H_

#include "MeshLoader.h"

#include <string>

namespace mesh {

using namespace std;

/**
 * Loads an Ansys mesh
 */
class AnsysMeshLoader: public MeshLoader {
public:
	/**
	 * Creates a new loader and reads from the specified Ansys folder
	 * @param folder The folder to the Ansys files
	 */
	AnsysMeshLoader(string folder);
	virtual ~AnsysMeshLoader();
};

}

#endif /* ANSYSMESHLOADER_H_ */

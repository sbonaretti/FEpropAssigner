#ifndef VTKPOLYDATACREATOR_H_
#define VTKPOLYDATACREATOR_H_

#include "vtkSmartPointer.h"
#include "vtkPolyData.h"
#include "Mesh.h"

#include <QSharedPointer>

namespace gui {

/**
 * Creates the vtkPolyData object for a mesh::Mesh
 */
class VtkPolyDataCreator {
public:
	/**
	 * Creates a new vtkPolyData object
	 * @param The mesh::Mesh the vtkPolyData is created from
	 * @return A pointer to the created vtkPolyData object
	 */
	static vtkSmartPointer<vtkPolyData> getVtkPolyData(QSharedPointer<mesh::Mesh> mesh);
};

}

#endif /* VTKPOLYDATACREATOR_H_ */

#include "VtkPolyDataCreator.h"

#include <iterator>
#include <vector>

#include "vtkSmartPointer.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkPolyData.h"
#include "vtkPolygon.h"
#include "vtkDoubleArray.h"
#include "vtkPointData.h"

namespace gui {

using namespace std;

vtkSmartPointer<vtkPolyData> VtkPolyDataCreator::getVtkPolyData(QSharedPointer<
		mesh::Mesh> mesh) {
	vtkSmartPointer<vtkPolyData> vtkPolygonData =
			vtkSmartPointer<vtkPolyData>::New();
	vtkSmartPointer<vtkPoints> vtkSurfacePoints =
			vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> vtkPolygons =
			vtkSmartPointer<vtkCellArray>::New();

	vector<mesh::Mesh::Point>* pointVector = mesh->getPointVector();
	vector<int>* nodeIds = mesh->getNodeIds();
	vector<mesh::Mesh::Triangle>* triangleVector = mesh->getTriangleVector();

	// add the surface nodes
	for (vector<int>::iterator it = nodeIds->begin(); it != nodeIds->end(); ++it) {
		int id = *it;
		mesh::Mesh::Point* point = &((*pointVector)[id - 1]);

		vtkIdType vtkId = id;
		vtkSurfacePoints->InsertPoint(vtkId, point->x, point->y, point->z);
	}

	// get the triangles
	for (vector<mesh::Mesh::Triangle>::iterator it = triangleVector->begin(); it
			!= triangleVector->end(); ++it) {
		vtkSmartPointer<vtkPolygon> polygon =
				vtkSmartPointer<vtkPolygon>::New();
		polygon->GetPointIds()->SetNumberOfIds(3);
		polygon->GetPointIds()->SetId(0, it->a);
		polygon->GetPointIds()->SetId(1, it->b);
		polygon->GetPointIds()->SetId(2, it->c);
		vtkPolygons->InsertNextCell(polygon);
	}

	vtkPolygonData->SetPoints(vtkSurfacePoints);
	vtkPolygonData->SetPolys(vtkPolygons);

	return vtkPolygonData;
}

}

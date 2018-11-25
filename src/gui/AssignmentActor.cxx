#include "AssignmentActor.h"

#include <vector>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkLookupTable.h>
#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkGlyph3D.h>
#include <vtkColorTransferFunction.h>
#include <vtkPointData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkMaskPoints.h>
#include <vtkScalarBarActor.h>
#include <vtkPropAssembly.h>
#include <QSharedPointer>

#include "Mesh.h"
#include "VtkPolyDataCreator.h"
#include "NodePropertyAssigner.h"

namespace gui {

using namespace std;

AssignmentActor* AssignmentActor::New(
		assignment::NodePropertyAssigner* nodePropertyAssigner) {
	AssignmentActor* assembly = (AssignmentActor*) vtkPropAssembly::New();
	vtkSmartPointer<vtkActor> bubbles = vtkSmartPointer<vtkActor>::New();

	QSharedPointer<mesh::Mesh> mesh = nodePropertyAssigner->getMesh();

	vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();

	vector<mesh::Mesh::Point>* pointVector = mesh->getPointVector();
	vector<double>* values = nodePropertyAssigner->getNodeValues();

	vtkLookupTable* lut = vtkLookupTable::New();
	lut->SetTableRange (nodePropertyAssigner->getMinValue(), nodePropertyAssigner->getMaxValue()); // number range
	lut->SetHueRange (0.0, 0.67); // color range <----
	lut->Build();
	lut->SetNumberOfColors(256);

	vtkSmartPointer<vtkUnsignedCharArray> colors = vtkSmartPointer<
			vtkUnsignedCharArray>::New();
	colors->SetName("colors");
	colors->SetNumberOfComponents(3);
	int maxPoints = 6000;
	int stepSize = (pointVector->size() / maxPoints);
	if (stepSize < 1)
		stepSize = 1;

	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	for (int i = 0; i < pointVector->size(); i += stepSize) {
		points->InsertNextPoint(pointVector->at(i).x, pointVector->at(i).y,
				pointVector->at(i).z);
		double value = (*values)[i];

		double colord[3];
		lut->GetColor(value, colord);
		unsigned char color[3];
		for (int c = 0; c < 3; c++) {
			color[c] = colord[c] * 255;
		}
		colors->InsertNextTupleValue(color);
	}

	polyData->SetPoints(points);

	polyData->GetPointData()->SetScalars(colors);

	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<
			vtkSphereSource>::New();

	vtkSmartPointer<vtkGlyph3D> glyph3D = vtkSmartPointer<vtkGlyph3D>::New();
	glyph3D->SetColorModeToColorByScalar();
	glyph3D->SetSource(sphereSource->GetOutput());
	glyph3D->SetInput(polyData);
	glyph3D->ScalingOff();
	glyph3D->Update();

	//Create a mapper
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<
			vtkPolyDataMapper>::New();
	mapper->SetInputConnection(glyph3D->GetOutputPort());

	bubbles->SetMapper(mapper);
	assembly->AddPart(bubbles);


	// Create color legend
	vtkSmartPointer<vtkScalarBarActor> scalarBar = vtkSmartPointer<
			vtkScalarBarActor>::New();
	scalarBar->SetLookupTable(mapper->GetLookupTable());
	//scalarBar->SetTitle("Legend");
	scalarBar->SetNumberOfLabels(4);
	//scalarBar->SetMaximumHeightInPixels(220);

	scalarBar->SetLookupTable(lut);

	assembly->AddPart(scalarBar);
	return assembly;
}

AssignmentActor::~AssignmentActor() {

}

}

#include "AssignmentTest.h"

#include <iostream>
#include <sstream>
#include "AbaqusMeshLoader.h"
#include "AnsysMeshLoader.h"
#include "CTImageReader.h"
#include "MeshLoader.h"
#include "DicomCTImageReader.h"
#include "MetaimageCTImageReader.h"
#include "VolumeCorrector.h"
#include "TricubicInterpolator.h"
#include "TrilinearInterpolator.h"
#include "NodePropertyAssigner.h"
#include "ElementPropertyAssigner.h"
#include "CTImage.h"
#include "Mesh.h"
#include "AbaqusElementPropertyWriter.h"
#include "AnsysElementPropertyWriter.h"
#include "AbaqusNodePropertyWriter.h"
#include "AnsysNodePropertyWriter.h"
#include <exception>

namespace test {

AssignmentTest::AssignmentTest() {


}

AssignmentTest::~AssignmentTest() {
	// TODO Auto-generated destructor stub
}

char* AssignmentTest::run() {
	using namespace ctimage;
	using namespace mesh;
	using namespace assignment;

	MetaimageCTImageReader* imageReader;
	AbaqusMeshLoader* meshLoader;
	try {
		imageReader = new MetaimageCTImageReader("./testData/bone1/image.mhd");
		meshLoader = new AbaqusMeshLoader("./testData/bone1/NetGenMeshAbaqusFormat.inp");
	} catch (exception& e) {
		return "Couldn't load test data. Probably missing test files?";
	}

	NodePropertyAssigner* assigner;
	try {
		assigner = new NodePropertyAssigner(imageReader->getCtImage(), meshLoader->getMesh(), TRILINEAR_INTERPOLATION, 1.0, 1.0);
	} catch (exception& e) {
		return "Error in assignment";
	}

	ASSERT("Wrong assignment value", assigner->getMaxValue() > 1649.0 && assigner->getMaxValue() < 1649.1);
	ASSERT("Wrong assignment value", assigner->getMinValue() > 4.9 && assigner->getMinValue() < 5.1 );

	ASSERT("Wrong assignment value",  assigner->getNodeValues()->at(132) > 93.4 && assigner->getNodeValues()->at(132) < 93.5);
	ASSERT("Wrong assignment value",  assigner->getNodeValues()->at(1000) > 190.7 && assigner->getNodeValues()->at(1000) < 190.8);
	ASSERT("Wrong assignment value",  assigner->getNodeValues()->at(3000) > 65.0 && assigner->getNodeValues()->at(3000) < 65.2);

	delete assigner;

	ElementPropertyAssigner assigner2(imageReader->getCtImage(), meshLoader->getMesh(), TRILINEAR_INTERPOLATION, 1.0, 1.0);
	assigner = &assigner2;

	ASSERT("Wrong assignment value", assigner->getMaxValue() > 1649.0 && assigner->getMaxValue() < 1649.1);
	ASSERT("Wrong assignment value", assigner->getMinValue() > 4.9 && assigner->getMinValue() < 5.1 );

	ASSERT("Wrong assignment value",  assigner->getNodeValues()->at(132) > 93.4 && assigner->getNodeValues()->at(132) < 93.5);
	ASSERT("Wrong assignment value",  assigner->getNodeValues()->at(1000) > 190.7 && assigner->getNodeValues()->at(1000) < 190.8);
	ASSERT("Wrong assignment value",  assigner->getNodeValues()->at(3000) > 65.0 && assigner->getNodeValues()->at(3000) < 65.2);

	return 0;
}

}

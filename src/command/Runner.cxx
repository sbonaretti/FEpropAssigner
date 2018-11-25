#include "Runner.h"

#include <string>
#include <iostream>
#include <exception>

#include <QSharedPointer>

#include "test/FEMPropertyAssignerTest.h"
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

namespace command {

using namespace std;
using namespace ctimage;
using namespace mesh;

Runner::Runner(int argc, char** argv) {
	string result = parser.parse(argc, argv);
	if (!result.empty()) {
		cout << result << "\n";
		exit(EXIT_FAILURE);
	}

	if (!parser.getTestRun()) {
		if (parser.getAbaqusInputFile().empty()
				&& parser.getAnsysInputFolder().empty()) {
			cout << "Specify mesh input" << endl;
			exit(EXIT_FAILURE);
		}
		if (parser.getDicomInputFolder().empty()
				&& parser.getMetaimageInputFile().empty()) {
			cout << "Specify ct image input" << endl;
			exit(EXIT_FAILURE);
		}
	}
}

void Runner::run() {
	if (parser.getTestRun()) {
		// run Tests
		test::FEMPropertyAssignerTest test;
		char* result = test.run();
		if (result) {
			cout << "	ERROR: " << result << endl;
			exit(EXIT_FAILURE);
		} else {
			cout << "All Tests passed!" << endl;
			exit(EXIT_SUCCESS);
		}
	}

	// run the whole routine
	CTImageReader* imageReader;
	MeshLoader* meshLoader;
	try {
		QSharedPointer<CTImage> ctImage;
		QSharedPointer<Mesh> mesh;

		if (!parser.getAnsysInputFolder().empty()) {
			// load Ansys mesh
			meshLoader = new AnsysMeshLoader(parser.getAnsysInputFolder());
		} else if (!parser.getAbaqusInputFile().empty()) {
			// load Abaqus mesh
			meshLoader = new AbaqusMeshLoader(parser.getAbaqusInputFile());
		}
		if (!parser.getDicomInputFolder().empty()) {
			// load dicom stack
			imageReader = new DicomCTImageReader(parser.getDicomInputFolder());
		} else if (!parser.getMetaimageInputFile().empty()) {
			// load metaimage
			imageReader = new MetaimageCTImageReader(
					parser.getMetaimageInputFile());
		}

		ctImage = imageReader->getCtImage();
		mesh = meshLoader->getMesh();

		if (parser.getExtrusionCount() > 0) {
			VolumeCorrector volumeCorrector(ctImage, mesh,
					parser.getExtrusionCount());
			ctImage = volumeCorrector.getExtrudedCTImage();
		}

		assignment::NodePropertyAssigner* assigner;
		if (parser.getAssignmentMethod() == assignment::NODE_ASSIGNMENT) {
			assigner = new assignment::NodePropertyAssigner(ctImage, mesh, parser.getInterpolationMethod(), parser.getLawParameterA(), parser.getLawParameterB());
		} else {
			assigner = new assignment::ElementPropertyAssigner(ctImage, mesh, parser.getInterpolationMethod(), parser.getLawParameterA(), parser.getLawParameterB());
		}

		if (!parser.getAbaqusOutputFile().empty()) {
			// write abaqus file
			using namespace mesh;
			if (parser.getAssignmentMethod() == assignment::NODE_ASSIGNMENT) {
				AbaqusNodePropertyWriter writer(parser.getAbaqusOutputFile(), assigner);
				writer.write();
			} else {
				AbaqusElementPropertyWriter writer(parser.getAbaqusOutputFile(), (assignment::ElementPropertyAssigner*)assigner);
				writer.write();
			}
		}
		if (!parser.getAnsysOutputFile().empty()) {
			// write ansys file
			using namespace mesh;
			if (parser.getAssignmentMethod() == assignment::NODE_ASSIGNMENT) {
				AnsysNodePropertyWriter writer(parser.getAnsysOutputFile(), assigner);
				writer.write();
			} else {
				AnsysElementPropertyWriter writer(parser.getAnsysOutputFile(), (assignment::ElementPropertyAssigner*)assigner);
				writer.write();
			}
		}

	} catch (exception &e) {
		cout << "Error: " << e.what() << endl;
		exit(EXIT_FAILURE);
	}

}

Runner::~Runner() {
}

}

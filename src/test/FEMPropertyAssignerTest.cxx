#include "FEMPropertyAssignerTest.h"

#include "Interpolator3DTest.h"
#include "CTImageTest.h"
#include "TrilinearInterpolatorTest.h"
#include "TricubicInterpolatorTest.h"
#include "AssignmentTest.h"
#include "MeshLoaderTest.h"

namespace test {

char* FEMPropertyAssignerTest::run() {
	RUN(CTImageTest);
	RUN(Interpolator3DTest);
	RUN(TrilinearInterpolatorTest);
	RUN(TricubicInterpolatorTest);
	RUN(MeshLoaderTest);
	RUN(AssignmentTest);
	return 0;
}

FEMPropertyAssignerTest::FEMPropertyAssignerTest() {

}

FEMPropertyAssignerTest::~FEMPropertyAssignerTest() {

}

}

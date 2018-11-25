#include "TricubicInterpolatorTest.h"

#include "CTImageMock.h"
#include "CTImage.h"

#include <QSharedPointer>

namespace test {

using namespace ctimage;
using namespace mesh;

char* TricubicInterpolatorTest::run() {
	QSharedPointer<CTImage> ctImage(new CTImageMock());
	double* values = new double[ctImage->getNsum()];
	for (int i = 0; i < ctImage->getNsum(); i++) {
		values[i] = 15.2;
	}

	assignment::TricubicInterpolator interpolator(ctImage, values);

	// Test index function
	char* indexError = "Wrong index calculation";
	ASSERT(indexError, interpolator.getIndex(1, 0, 0) == 10);
	ASSERT(indexError, interpolator.getIndex(0, 0, 0) == 0);
	ASSERT(indexError, interpolator.getIndex(0, 0, 1) == 20*10);

	// Test cubic interpolation
	char* cubicError = "Wrong cubic interpolation";

	assignment::TricubicInterpolator::Sample sample[4];
	for (int i = 0; i < 4; i++) {
		sample[i].value = 5;
		sample[i].x = i;
	}
	double result = interpolator.interpolateCubic(sample, 1.45);
	ASSERT(cubicError, result == 5);

	for (int i = 0; i < 4; i++) {
		sample[i].value = i;
		sample[i].x = i + 1;
	}
	result = interpolator.interpolateCubic(sample, 2.5);
	ASSERT(cubicError, result == 1.5);

	sample[0].value = 1000;
	sample[1].value = 5;
	sample[2].value = 5;
	sample[3].value = 1000;

	result = interpolator.interpolateCubic(sample, 2.5);
	ASSERT(cubicError, result == -243.75);

	// Test bicubic interpolation
	char* bicubicError = "Wrong bicubic interpolation";

	Mesh::Point p;
	p.x = 110.2;
	p.y = 208.2;
	p.z = 3.3;
	interpolator.setUpCube(&p);
	result = interpolator.interpolateBicubic(interpolator.z0Index,
			interpolator.x0 + 0.2, interpolator.y0 + 0.2);
	ASSERT(bicubicError, result == 15.2);

	// Test tricubic interpolation
	char* tricubicError = "Wrong tricubic interpolation";

	for (int i = 0; i < ctImage->getNsum(); i++) {
		values[i] = i;
	}

	Mesh::Point point;
	point.x = 107.5;
	point.y = 204.5;
	point.z = 2.75;
	result = interpolator.getValue(&point);

	ASSERT(tricubicError, result == 316.5);

	delete[] values;

	return 0;
}

TricubicInterpolatorTest::TricubicInterpolatorTest() {
	// TODO Auto-generated constructor stub

}

TricubicInterpolatorTest::~TricubicInterpolatorTest() {
	// TODO Auto-generated destructor stub
}

}

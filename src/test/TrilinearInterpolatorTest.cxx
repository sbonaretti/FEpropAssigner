#include "TrilinearInterpolatorTest.h"

#include "CTImageMock.h"
#include "CTImage.h"
#include "TrilinearInterpolator.h"

#include <QSharedPointer>

namespace test {

using namespace ctimage;
using namespace mesh;

char* TrilinearInterpolatorTest::run() {
	QSharedPointer<CTImage> ctImage(new CTImageMock);
	double* values = new double[ctImage->getNsum()];
	for (int i = 0; i < ctImage->getNsum(); i++) {
		values[i] = i;
	}

	assignment::TrilinearInterpolator interpolator(ctImage, values);

	Mesh::Point point;
	point.x = 144.4;
	point.y = 207.7;
	point.z = 3.9;
	double value = interpolator.getValue(&point);

	stringstream ss;
	ss << value;
	ASSERT("Wrong interpolation result", value > 851 && value < 851.5);

	// Test out of bounds exception
	char* noException = "No exception thrown";
	try {
		Mesh::Point p2 = point;
		p2.x = 90;
		interpolator.getValue(&p2);
		FAIL(noException);
	} catch (exception& e) {
	}
	try {
		Mesh::Point p2 = point;
		p2.y = 199.9;
		interpolator.getValue(&p2);
		FAIL(noException);
	} catch (exception& e) {
	}
	try {
		Mesh::Point p2 = point;
		p2.z = 1;
		interpolator.getValue(&p2);
		FAIL(noException);
	} catch (exception& e) {
	}
	try {
		Mesh::Point p2 = point;
		p2.x = 200.3;
		interpolator.getValue(&p2);
		FAIL(noException);
	} catch (exception& e) {
	}
	try {
		Mesh::Point p2 = point;
		p2.y = 230.1;
		interpolator.getValue(&p2);
		FAIL(noException);
	} catch (exception& e) {
	}
	try {
		Mesh::Point p2 = point;
		p2.z = 4.1;
		interpolator.getValue(&p2);
		FAIL(noException);
	} catch (exception& e) {
	}

	delete[] values;

	return 0;
}

TrilinearInterpolatorTest::TrilinearInterpolatorTest() {
	// TODO Auto-generated constructor stub

}

TrilinearInterpolatorTest::~TrilinearInterpolatorTest() {
	// TODO Auto-generated destructor stub
}

}

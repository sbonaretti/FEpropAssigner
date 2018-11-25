#include "Interpolator3DTest.h"

#include "CTImage.h"
#include "CTImageMock.h"
#include "CTImageTest.h"
#include "Interpolator3D.h"

#include <QSharedPointer>

namespace test {

using namespace ctimage;
using namespace mesh;

class Interpolator3DMock: public assignment::Interpolator3D {
public:
	Interpolator3DMock(QSharedPointer<CTImage> , double*);
	int** setUpCubeMock(Mesh::Point*);
	double getValue(Mesh::Point*);
};
int** Interpolator3DMock::setUpCubeMock(Mesh::Point* point) {
	setUpCube(point);
	int** index = new int*[3];
	for (int i = 0; i < 3; i++) {
		index[i] = new int[2];
	}
	index[0][0] = x0Index;
	index[0][1] = x1Index;
	index[1][0] = y0Index;
	index[1][1] = y1Index;
	index[2][0] = z0Index;
	index[2][1] = z1Index;

	return index;
}
double Interpolator3DMock::getValue(Mesh::Point* point) {
	return 0;
}
Interpolator3DMock::Interpolator3DMock(QSharedPointer<CTImage> c, double* d) :
	assignment::Interpolator3D(c, d) {
}

char* Interpolator3DTest::run() {
	// Test cube search
	QSharedPointer<CTImageMock> ctImage(new CTImageMock);
	double* values = new double[ctImage->getNsum()];
	for (int i = 0; i < ctImage->getNsum(); i++) {
		values[i] = i;
	}

	Interpolator3DMock interpolator(ctImage, values);

	Mesh::Point point;
	point.x = 100.2;
	point.y = 202.9;
	point.z = 2.2;
	int** index = interpolator.setUpCubeMock(&point);
	char* wrongCube = "Didn't find right cube";
	for (int i = 0; i < 3; i++) {
		ASSERT(wrongCube, index[i][0] == 0 && index[i][1] == 1);
	}

	// test another cube
	point.x = 115.3;
	point.y = 204.1;
	point.z = 3.1;
	index = interpolator.setUpCubeMock(&point);
	ASSERT(wrongCube, index[0][0] = 3 && index[0][1] == 4);
	ASSERT(wrongCube, index[1][0] = 1 && index[1][1] == 2);
	ASSERT(wrongCube, index[2][0] = 2 && index[2][1] == 3);

	delete[] values;

	return 0;
}

Interpolator3DTest::Interpolator3DTest() {
	// TODO Auto-generated constructor stub

}

Interpolator3DTest::~Interpolator3DTest() {
	// TODO Auto-generated destructor stub
}

}

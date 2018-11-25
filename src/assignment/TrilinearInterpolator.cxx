#include "TrilinearInterpolator.h"
#include "Interpolator3D.h"

namespace assignment {

TrilinearInterpolator::TrilinearInterpolator(QSharedPointer<CTImage> ctImage,
		double* values) :
	Interpolator3D(ctImage, values) {
}

double TrilinearInterpolator::interpolate(double x0, double x1, double v0,
		double v1, double x) {
	double d = x1 - x0;
	if (d == 0)
		return v0;
	else
		return ((x - x0) / d) * v1 + ((x1 - x) / d) * v0;
}

double TrilinearInterpolator::getValue(Mesh::Point* point) {
	setUpCube(point);

	// front
	double value0 = interpolate(x0, x1, values[getIndex(x0Index, y0Index,
			z0Index)], values[getIndex(x1Index, y0Index, z0Index)], (*point).x);
	double value1 = interpolate(x0, x1, values[getIndex(x0Index, y1Index,
			z0Index)], values[getIndex(x1Index, y1Index, z0Index)], (*point).x);
	double resultFront = interpolate(y0, y1, value0, value1, (*point).y);

	// back
	value0 = interpolate(x0, x1, values[getIndex(x0Index, y0Index, z1Index)],
			values[getIndex(x1Index, y0Index, z1Index)], (*point).x);
	value1 = interpolate(x0, x1, values[getIndex(x0Index, y1Index, z1Index)],
			values[getIndex(x1Index, y1Index, z1Index)], (*point).x);
	double resultBack = interpolate(y0, y1, value0, value1, (*point).y);

	double result = interpolate(z0, z1, resultFront, resultBack, (*point).z);

	return result;
}

TrilinearInterpolator::~TrilinearInterpolator() {

}

}

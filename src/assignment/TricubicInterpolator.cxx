#include "TricubicInterpolator.h"
#include "Interpolator3D.h"
#include "CTImage.h"
#include "Mesh.h"

namespace assignment {

TricubicInterpolator::TricubicInterpolator(QSharedPointer<CTImage> ctImage,
		double* values) :
	Interpolator3D(ctImage, values) {
}

double TricubicInterpolator::getValue(Mesh::Point* point) {
	setUpCube(point);

	// interpolate bicubic in all 4 z-slices

	double result[4];
	for (int z = 0; z < 4; z++) {
		int zIndex = (z0Index - 1) + z;

		// check if the cube is at the edge
		if (zIndex < 0)
			zIndex = 0;
		if (zIndex >= ctImage->getNz())
			zIndex = ctImage->getNz() - 1;

		result[z] = interpolateBicubic(zIndex, point->x, point->y);
	}

	// interpolate a last time in z direction

	Sample sample[4];
	double d = ctImage->getSz();
	for (int z = 0; z < 4; z++) {
		sample[z].value = result[z];
		sample[z].x = (z0 - d) + z * d;
	}

	return interpolateCubic(sample, point->z);
}

TricubicInterpolator::~TricubicInterpolator() {

}

// interpolate cubic. x must lie between samples[1] and samples[2]
double TricubicInterpolator::interpolateCubic(Sample* samples, double xCoord) {
	// Sample[4]
	double mu = (xCoord - samples[1].x) / (samples[2].x - samples[1].x);
	double mu2 = mu * mu;

	double a0 = samples[3].value - samples[2].value - samples[0].value
			+ samples[1].value;
	double a1 = samples[0].value - samples[1].value - a0;
	double a2 = samples[2].value - samples[0].value;
	double a3 = samples[1].value;

	return (a0 * mu * mu2 + a1 * mu2 + a2 * mu + a3);
}

double TricubicInterpolator::interpolateBicubic(int zIndex, double xCoord,
		double yCoord) {
	// interpolate 4 times in x direction

	double result[4];
	Sample sample[4];
	for (int y = 0; y < 4; y++) {
		int yIndex = (y0Index - 1) + y;

		// check if the cube is at the edge
		if (yIndex < 0)
			yIndex = 0;
		if (yIndex >= ctImage->getNy())
			yIndex = ctImage->getNy() - 1;

		for (int x = 0; x < 4; x++) {
			int xIndex = (x0Index - 1) + x;

			// check if the cube is at the edge
			if (xIndex < 0)
				xIndex = 0;
			if (xIndex >= ctImage->getNx())
				xIndex = ctImage->getNx() - 1;

			double d = ctImage->getSx();
			sample[x].x = (x0 - d) + x * d;
			sample[x].value = values[getIndex(xIndex, yIndex, zIndex)];
		}
		result[y] = interpolateCubic(sample, xCoord);
	}

	// now interpolate once in y direction
	for (int y = 0; y < 4; y++) {
		double d = ctImage->getSy();
		sample[y].x = (y0 - d) + y * d;
		sample[y].value = result[y];
	}

	return interpolateCubic(sample, yCoord);
}

}

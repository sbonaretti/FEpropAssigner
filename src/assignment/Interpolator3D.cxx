#include "Interpolator3D.h"
#include "CTImage.h"
#include "Mesh.h"

namespace assignment {

Interpolator3D::Interpolator3D(QSharedPointer<CTImage> ctImage, double* values) {
	this->ctImage = ctImage;
	this->values = values;
}

int Interpolator3D::getIndex(int x, int y, int z) {
	int value = y;
	value += x * ctImage->getNy();
	value += z * ctImage->getNx() * ctImage->getNy();
	return value;
}

void Interpolator3D::setUpCube(Mesh::Point* point) {
	x0Index = 0;
	x1Index = 0;
	y0Index = 0;
	y1Index = 0;
	z0Index = 0;
	z1Index = 0;

	bool outOfBounds = false;
	if (ctImage->getX0() < (*point).x) {
		for (x1 = ctImage->getX0(); x1 < (*point).x; x1 += ctImage->getSx()) {
			x1Index++;
			if (x1Index >= ctImage->getNx()) {
				outOfBounds = true;
				break;
			}
		}
		x0 = x1 - ctImage->getSx();
		x0Index = x1Index - 1;
	} else {
		outOfBounds = true;
		x0 = ctImage->getSx();
		x1 = x0;
	}

	if (ctImage->getY0() < (*point).y) {
		for (y1 = ctImage->getY0(); y1 < (*point).y; y1 += ctImage->getSy()) {
			y1Index++;
			if (y1Index >= ctImage->getNy()) {
				outOfBounds = true;
				break;
			}
		}
		y0 = y1 - ctImage->getSy();
		y0Index = y1Index - 1;
	} else {
		outOfBounds = true;
		y0 = ctImage->getSy();
		y1 = y0;
	}

	if (ctImage->getZ0() < (*point).z) {
		for (z1 = ctImage->getZ0(); z1 < (*point).z; z1 += ctImage->getSz()) {
			z1Index++;
			if (z1Index >= ctImage->getNz()) {
				outOfBounds = true;
				break;
			}
		}
		z0 = z1 - ctImage->getSz();
		z0Index = z1Index - 1;
	} else {
		outOfBounds = true;
		z0 = ctImage->getSz();
		z1 = z0;
	}

	if (outOfBounds) {
		class OutOfBoundsException: public exception {
			virtual const char* what() const throw () {
				return "The FEM model is outside the CT image";
			}
		} outOfBoundsException;
		throw outOfBoundsException;
	}
}

Interpolator3D::~Interpolator3D() {

}

}

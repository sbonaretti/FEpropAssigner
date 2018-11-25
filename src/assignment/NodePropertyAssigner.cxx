#include "NodePropertyAssigner.h"
#include "Interpolator3D.h"
#include "TricubicInterpolator.h"
#include "TrilinearInterpolator.h"
#include "Mesh.h"
#include "CTImage.h"

#include <vector>
#include <cmath>
#include <cstdio>
#include <cfloat>
#include <utility>

namespace assignment {

using namespace std;

NodePropertyAssigner::NodePropertyAssigner(QSharedPointer<CTImage> ctImage,
		QSharedPointer<Mesh> mesh, int interpolationMethod, double a, double b) {
	this->a = a;
	this->b = b;
	this->mesh = mesh;
	minValue = DBL_MAX;
	maxValue = DBL_MIN;

	double* rhoAsh = this->ashDensityRev(ctImage->getNsum(),
			ctImage->getPixelData());

	vector<Mesh::Point>* nodes = mesh->getPointVector();

	Interpolator3D* i;
	if (interpolationMethod == TRICUBIC_INTERPOLATION) {
		i = new TricubicInterpolator(ctImage, rhoAsh);
	} else {
		i = new TrilinearInterpolator(ctImage, rhoAsh);
	}
	QSharedPointer<Interpolator3D> interpolator(i);

	for (int i = 0; i < (*nodes).size(); i++) {
		int nodeNr = mesh->getNodeIds()->at(i);

		double result = interpolator->getValue(&(*nodes)[i]);
		if (result > maxValue)
			maxValue = result;
		if (result < minValue)
			minValue = result;

		this->nodeValues.push_back(result);
	}

	delete [] rhoAsh;
}

double* NodePropertyAssigner::ashDensityRev(int n, CTImage::CTPixelType* ct) {
	double* rhoAsh = new double[n];
	for (int i = 0; i < n; i++) {
		rhoAsh[i] = ct[i] / 2000.0 * 1.8;
		if (rhoAsh[i] < 0)
			rhoAsh[i] = 0;
	}

	double* eAsh = rhoAsh;
	double e_min = 5;

	for (int i = 0; i < n; i++) {
		double value = this->a * pow(rhoAsh[i], this->b) * 1000; // a * rhoAsh^b * 1000
		if (value < e_min)
			value = e_min;
		eAsh[i] = value;
	}

	return eAsh;
}

NodePropertyAssigner::~NodePropertyAssigner() {

}

vector<double>* NodePropertyAssigner::getNodeValues() {
	return &this->nodeValues;
}

QSharedPointer<Mesh> NodePropertyAssigner::getMesh() {
	return this->mesh;
}

double NodePropertyAssigner::getMinValue() {
	return minValue;
}

double NodePropertyAssigner::getMaxValue() {
	return maxValue;
}

}

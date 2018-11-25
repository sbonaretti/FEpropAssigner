#ifndef TRILINEARINTERPOLATOR_H_
#define TRILINEARINTERPOLATOR_H_

#include "Interpolator3D.h"

#include "CTImage.h"
#include "Mesh.h"

namespace assignment {

/**
 * Interpolates a mesh::Mesh::Point trilinear
 * @note http://en.wikipedia.org/wiki/Trilinear_interpolation
 */
class TrilinearInterpolator: public Interpolator3D {
public:
	/**
	 * Creates a new trilinear interpolator with a given ctimage::CTImage and the values used for the interpolation
	 * @param ctImage The ctimage::CTImage, which is used only for morphologic information. The values of the ctimage::CTImage are not used for the interpolation
	 * @param values The values used for the interpolation. The values array needs to be the size of the pixel count of the ctimage::CTImage
	 */
	TrilinearInterpolator(QSharedPointer<CTImage> , double*);
	double getValue(Mesh::Point*);
	virtual ~TrilinearInterpolator();
private:
	/**
	 * Returns the result of linear interpolation between two points
	 * @param x0 The first sample point's coordinate
	 * @param x1 The second sample point's coordinate
	 * @param v0 The first sample point's value
	 * @param v1 The second sample point's value
	 * @param x The coordinate of the point to be interpolated
	 * @return The interpolation result
	 */
	double interpolate(double x0, double x1, double v0, double v1, double x);
};

}

#endif /* TRILINEARINTERPOLATOR_H_ */

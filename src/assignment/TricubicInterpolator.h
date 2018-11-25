#ifndef TRICUBICINTERPOLATOR_H_
#define TRICUBICINTERPOLATOR_H_

#include "Interpolator3D.h"

#include "TricubicInterpolatorTest.h"

namespace test {
class TricubicInterpolatorTest;
}

namespace assignment {

/**
 * Interpolates a mesh::Mesh::Point tricubic
 * @note http://en.wikipedia.org/wiki/Tricubic_interpolation
 */
class TricubicInterpolator: public Interpolator3D {
	friend class test::TricubicInterpolatorTest;
public:
	/**
	 * Creates a new tricubic interpolator with a given ctimage::CTImage and the values used for the interpolation
	 * @param ctImage The ctimage::CTImage, which is used only for morphologic information. The values of the ctimage::CTImage are not used for the interpolation
	 * @param values The values used for the interpolation. The values array needs to be the size of the pixel count of the ctimage::CTImage
	 */
	TricubicInterpolator(QSharedPointer<CTImage>, double*);
	double getValue(Mesh::Point*);
	virtual ~TricubicInterpolator();
private:
	/*
	 * A sample point used for cubic interpolation
	 */
	struct Sample {
		double x;
		double value;
	};

	/**
	 * Returns the result of cubic interpolation between 4 Sample points
	 * @warning The length of sample must be 4 and the value x must be within the 2nd and the 3rd sample
	 * @param sample The Sample array containing 4 samples
	 * @param x The point to be interpolated
	 * @return The interpolation result
	 */
	double interpolateCubic(Sample* sample, double x);
	/**
	 * Returns the result of bicubic interpolation inside a z-slice of the 4x4x4 grid surrounding the point to be interpolated
	 * @param zIndex The z-index of the slice
	 * @param x The x-coordinate of the point to be interpolated
	 * @param y The y-coordinate of the point to be interpolated
	 * @return The interpolation result
	 */
	double interpolateBicubic(int zIndex, double x, double y);
};

}

#endif /* TRICUBICINTERPOLATOR_H_ */

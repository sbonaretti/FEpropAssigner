#ifndef INTERPOLATOR3D_H_
#define INTERPOLATOR3D_H_

#include "CTImage.h"
#include "Mesh.h"

#include <QSharedPointer>

namespace assignment {

const int TRILINEAR_INTERPOLATION = 1;
const int TRICUBIC_INTERPOLATION = 2;

using namespace ctimage;
using namespace mesh;

/**
 * Interpolates a mesh::Mesh::Point inside a CTImage
 */
class Interpolator3D {
protected:
	/** A pointer to the ctimage::CTImage */
	QSharedPointer<CTImage> ctImage;
	/** The values to be used for the interpolation */
	double* values;
	/** The first edge x-coordinate of the cube surrounding the current interpolation point */
	double x0;
	/** The second x-coordinate of the cube surrounding the current interpolation point */
	double x1;
	/** The first edge y-coordinate of the cube surrounding the current interpolation point */
	double y0;
	/** The second edge y-coordinate of the cube surrounding the current interpolation point */
	double y1;
	/** The first edge z-coordinate of the cube surrounding the current interpolation point */
	double z0;
	/** The second edge z-coordinate of the cube surrounding the current interpolation point */
	double z1;
	/** The first x-coordinate edge index value of the cube surrounding the current interpolation point */
	int x0Index;
	/** The second x-coordinate edge index value of the cube surrounding the current interpolation point */
	int x1Index;
	/** The first y-coordinate edge index value of the cube surrounding the current interpolation point */
	int y0Index;
	/** The second y-coordinate edge index value of the cube surrounding the current interpolation point */
	int y1Index;
	/** The first z-coordinate edge index value of the cube surrounding the current interpolation point */
	int z0Index;
	/** The second z-coordinate edge index value of the cube surrounding the current interpolation point */
	int z1Index;
	/**
	 * Gets the index for the values array, given xyz coordinate indexes
	 * @param x x index
	 * @param y y index
	 * @param z z index
	 * @return The index value
	 * */
	int getIndex(int x, int y, int z);
	/**
	 * Sets the members for the cube surrounding the current interpolation point
	 * @param p The point to be interpolated
	 */
	void setUpCube(Mesh::Point* p);
public:
	/**
	 * Creates a new interpolator with a given ctimage::CTImage and the values used for the interpolation
	 * @param ctImage The ctimage::CTImage, which is used only for morphologic information. The values of the ctimage::CTImage are not used for the interpolation
	 * @param values The values used for the interpolation. The values array needs to be the size of the pixel count of the ctimage::CTImage
	 */
	Interpolator3D(QSharedPointer<CTImage> ctImage, double* values);

	/**
	 * Gets the interpolated value for a specified mesh::Mesh::Point
	 * @param p The point to be interpolated
	 */
	virtual double getValue(Mesh::Point* p) = 0;
	virtual ~Interpolator3D();
};

}

#endif /* INTERPOLATOR3D_H_ */

#ifndef VOLUMECORRECTOR_H_
#define VOLUMECORRECTOR_H_

#include "CTImage.h"
#include "Mesh.h"

#include <QSharedPointer>

namespace ctimage {

/**
 * Extrudes a ctimage::CTImage in order to completely cover a mesh::Mesh
 * @warning This class doesn't alter the provided ctimage::CTImage. It instead creates a new ctimage::CTImage
 */
class VolumeCorrector {
public:
	/**
	 * @param ctImage The ctimage::CTImage to be extruded
	 * @param mesh The corresponding mesh::Mesh
	 */
	VolumeCorrector(QSharedPointer<CTImage> ctImage, QSharedPointer<mesh::Mesh> mesh, int);
	/**
	 * Returns the extruded ctimage::CTImage
	 * @return The extruded ctimage::CTImage
	 */
	QSharedPointer<CTImage> getExtrudedCTImage();
	virtual ~VolumeCorrector();
private:
	/**
	 * A 2-Dimensional point
	 */
	typedef struct {
		double x;
		double y;
	} Point2D;

	/**
	 * A connection of two points
	 */
	typedef struct {
		Point2D a;
		Point2D b;
	} Line;

	/**
	 * The iteration count for the image extrusion
	 */
	int extrusionCount;
	/**
	 * The mesh that corresponds to the specified ctimage::CTImage
	 */
	QSharedPointer<mesh::Mesh> mesh;
	/**
	 * The extruded ctimage::CTImage
	 */
	QSharedPointer<CTImage> extrudedCTImage;

	/**
	 * Run the extrusion routine
	 */
	void extrude();
	/**
	 * Fills a bool array which is true where the points are inside a tetrahedron
	 * @param The bool array that will be filled
	 * @warning The bool array must be the size of the amount of pixels in the ctimage::CTImage
	 */
	void inTetrahedron(bool*);
	void chopSurface(bool *ctBool, bool *ctSlim, bool *ctPeeled);
	void extendSurface(CTImage::CTPixelType* E, bool* c,
			CTImage::CTPixelType* Ex, bool* cx);
};

}

#endif /* VOLUMECORRECTOR_H_ */

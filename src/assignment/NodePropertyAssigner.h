#ifndef TEMPERATUREASSIGNER_H_
#define TEMPERATUREASSIGNER_H_

#include "Mesh.h"
#include "CTImage.h"

#include <vector>
#include <map>

#include <QSharedPointer>

/**
 * Contains classes that are related to the property assignment
 */
namespace assignment {

const int NODE_ASSIGNMENT = 1;
const int ELEMENT_ASSIGNMENT = 2;

using namespace ctimage;
using namespace mesh;

/**
 * @brief Assigns CT image data to the nodes of a mesh::Mesh
 */
class NodePropertyAssigner {
public:
	/**
	 * Creates a new assigner
	 * @param ctimage The ctimage::CTImage
	 * @param mesh The mesh::Mesh
	 * @param interpolationMethod The interpolation method
	 * @param a The first parameter for the assignment law
	 * @param b The second parameter for the assignment law
	 */
	NodePropertyAssigner(QSharedPointer<CTImage> ctimage,
			QSharedPointer<Mesh> mesh, int interpolationMethod, double a,
			double b);
	/**
	 * Gets the mesh::Mesh that is used for the assignment
	 */
	QSharedPointer<Mesh> getMesh();
	/**
	 * Gets the minimum value of the assignment
	 */
	double getMinValue();
	/**
	 * Gets the maximum value of the assignment
	 */
	double getMaxValue();
	/**
	 * Gets the assigned node values
	 */
	vector<double>* getNodeValues();
	virtual ~NodePropertyAssigner();
protected:
	/**
	 * The mesh::Mesh the values are assigned to
	 */
	QSharedPointer<Mesh> mesh;
	/**
	 * A vector containing the assigned node values
	 */
	vector<double> nodeValues;
	/**
	 * The minimum value of the assignment
	 */
	double minValue;
	/**
	 * The maximum value of the assignment
	 */
	double maxValue;
	/**
	 * The first parameter for the assignment law
	 */
	double a;
	/**
	 * The second parameter for the assignment law
	 */
	double b;
	/**
	 * Returns the calculated density of a CT image's pixels
	 * @param n The total pixel count
	 * @param ct The CT image's pixels
	 * @return The calculated density values
	 */
	double* ashDensityRev(int n, CTImage::CTPixelType* ct);
};

}

#endif /* TEMPERATUREASSIGNER_H_ */

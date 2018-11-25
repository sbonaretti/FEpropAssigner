#ifndef ELEMENTPROPERTYASSIGNER_H_
#define ELEMENTPROPERTYASSIGNER_H_

#include "NodePropertyAssigner.h"

#include <QSharedPointer>

namespace assignment {

/**
 * @brief Assigns CT image data to the elements of a mesh::Mesh
 */
class ElementPropertyAssigner: public NodePropertyAssigner {
public:
	/**
	 * Creates a new assigner
	 * @param ctimage The ctimage::CTImage
	 * @param mesh The mesh::Mesh
	 * @param interpolationMethod The interpolation method
	 * @param a The first parameter for the assignment law
	 * @param b The second parameter for the assignment law
	 */
	ElementPropertyAssigner(QSharedPointer<CTImage> , QSharedPointer<Mesh>, int interpolationMethod, double a, double b);
	/**
	 * Gets the calculated values in ascending element id order
	 * @return The vector containing the element values
	 */
	vector<double>* getElementValues();
protected:
	 /** The calculated element values */
	vector<double> elementValues;
};

}

#endif /* ELEMENTPROPERTYASSIGNER_H_ */

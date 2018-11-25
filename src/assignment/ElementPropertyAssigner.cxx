#include "ElementPropertyAssigner.h"
#include "Mesh.h"

#include <map>
#include <utility>

namespace assignment {

ElementPropertyAssigner::ElementPropertyAssigner(
		QSharedPointer<CTImage> ctImage, QSharedPointer<Mesh> mesh, int interpolationMethod, double a, double b) :
	NodePropertyAssigner(ctImage, mesh, interpolationMethod, a, b) {
	// create link between nodeIds and assignment values
	map<int, double> nodeValues;
	for (int n = 0; n < mesh->getNodeIds()->size(); n++) {
		nodeValues.insert(make_pair(mesh->getNodeIds()->at(n),
				this->getNodeValues()->at(n)));
	}

	map<int, Mesh::Element>* elements = mesh->getElements();

	for (map<int, Mesh::Element>::iterator it = elements->begin(); it
			!= elements->end(); ++it) {
		double value = 0;
		Mesh::Element* element = &(*it).second;

		for (vector<int>::iterator it = element->intermediateNodes.begin(); it
				!= element->intermediateNodes.end(); ++it) {
			value += nodeValues[*it];
		}
		for (vector<int>::iterator it = element->triangleNodes.begin(); it
				!= element->triangleNodes.end(); ++it) {
			value += nodeValues[*it];
		}

		value /= element->intermediateNodes.size()
				+ element->triangleNodes.size();

		elementValues.push_back(value);
	}
}

vector<double>* ElementPropertyAssigner::getElementValues() {
	return &elementValues;
}

}

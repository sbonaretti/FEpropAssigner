#include "AbaqusMeshLoader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <strstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <cassert>
#include <map>
#include <utility>

namespace mesh {

AbaqusMeshLoader::AbaqusMeshLoader(string file) {
	int ELEMENT_NODE_N = 10;

	ifstream input(file.c_str(), ios::in);
	if (!input.good()) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "Could not open Abaqus file";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	//auto_ptr<Mesh> m( new Mesh());
	QSharedPointer<Mesh> qpoint(new Mesh());
	this->mesh = qpoint;
	int READ_NODE = 1;
	int READ_ELEMENT = 2;
	int read = 0;

	while (true) {
		string line;
		std::getline(input, line);
		if (input.eof())
			break;

		if (line.find("*Node") != line.npos) {
			read = READ_NODE;
			continue;
		} else if (line.find("*Element") != line.npos) {
			read = READ_ELEMENT;
			continue;
		}

		if (read == READ_NODE) {
			vector<string> tokens;
			istrstream iss(line.c_str());
			copy(istream_iterator<string> (iss), istream_iterator<string> (),
					back_inserter<vector<string> > (tokens));

			Mesh::Point point;
			for (unsigned i = 0; i < tokens.size(); i++) {
				string token = tokens[i];

				istringstream stream;
				stream.str(token);
				if (i == 0) {
					int nodeNr;
					stream >> nodeNr;
					mesh->nodeIds.push_back(nodeNr);
				} else {
					double coord;
					stream >> coord;

					switch (i) {
					case 1: // x coord
						point.x = coord;
						break;
					case 2: // y coord
						point.y = coord;
						break;
					case 3: // z coord
						point.z = coord;
						break;
					}
				}
			}
			mesh->getPointVector()->push_back(point);
		} else if (read == READ_ELEMENT) {
			vector<string> tokens;
			istrstream iss(line.c_str());
			copy(istream_iterator<string> (iss), istream_iterator<string> (),
					back_inserter<vector<string> > (tokens));

			while (tokens.size() - 1 < ELEMENT_NODE_N) { // the elements are usally spread over 2 lines -> bring them together
				vector<string> nextTokens;
				string nextLine;
				std::getline(input, nextLine);
				istrstream iss(nextLine.c_str());
				copy(istream_iterator<string> (iss),
						istream_iterator<string> (), back_inserter<vector<
								string> > (nextTokens));
				tokens.insert(tokens.end(), nextTokens.begin(),
						nextTokens.end()); // append the new tokens to the vector
				assert(tokens.size() > 0);
			}

			Mesh::Element element;
			int elementId;

			for (int i = 0; i < tokens.size(); i++) {
				istringstream stream;
				stream.str(tokens[i]);
				int value;
				stream >> value;
				if (i == 0) {
					// Element Id
					elementId = value;
				} else if (i < 5) {
					// Tetrahedron edge
					element.triangleNodes.push_back(value);
				} else {
					// Intermediate node
					element.intermediateNodes.push_back(value);
				}
			}

			mesh->elements.insert(make_pair(elementId, element));
		}
	}
	input.close();
	findSurfaceNodes();
}

// Save triangles together with the element Id
struct ElementTriangle {
	int elementId;
	Mesh::Triangle triangle;
};
void insertTriangle(ElementTriangle* etriangle, map<int,
		vector<ElementTriangle> >* triangleMap, int a, int b, int c) {
	// sort in ascending order
	if (a > b)
		swap(a, b);
	if (b > c)
		swap(b, c);
	if (b < a)
		swap(a, b);

	etriangle->triangle.a = a;
	etriangle->triangle.b = b;
	etriangle->triangle.c = c;

	vector<ElementTriangle>* etriangles = &((*triangleMap)[a]);

	etriangles->push_back(*etriangle);
}

bool same(Mesh::Triangle* t1, Mesh::Triangle* t2) {
	return (t1->a == t2->a && t1->b == t2->b && t1->c == t2->c);
}

void AbaqusMeshLoader::findSurfaceNodes() {
	map<int, vector<ElementTriangle> > allTriangles; // map with first triangle node as index

	for (map<int, Mesh::Element>::iterator it = mesh->elements.begin(); it
			!= mesh->elements.end(); ++it) {
		int elementId = (*it).first;

		vector<int>* tetrahedron = &(*it).second.triangleNodes;

		// add triangle permutations
		ElementTriangle etriangle;
		etriangle.elementId = elementId;

		insertTriangle(&etriangle, &allTriangles, tetrahedron->at(0),
				tetrahedron->at(1), tetrahedron->at(2)); // 012
		insertTriangle(&etriangle, &allTriangles, tetrahedron->at(0),
				tetrahedron->at(1), tetrahedron->at(3)); // 013
		insertTriangle(&etriangle, &allTriangles, tetrahedron->at(0),
				tetrahedron->at(2), tetrahedron->at(3)); // 023
		insertTriangle(&etriangle, &allTriangles, tetrahedron->at(1),
				tetrahedron->at(2), tetrahedron->at(3)); // 123
	}

	for (map<int, vector<ElementTriangle> >::iterator it = allTriangles.begin(); it
			!= allTriangles.end(); ++it) {
		vector<ElementTriangle>* etriangles = &(*it).second;
		for (int i = 0; i < etriangles->size(); i++) {
			if (etriangles->at(i).elementId == -1)
				continue;
			bool unique = true;
			for (int j = i + 1; j < etriangles->size(); j++) {
				if (same(&etriangles->at(i).triangle,
						&etriangles->at(j).triangle)) {
					unique = false;
					etriangles->at(j).elementId = -1;
				}
			}
			if (unique) {
				mesh->triangleVector.push_back(etriangles->at(i).triangle);
			}
		}
	}
}

AbaqusMeshLoader::~AbaqusMeshLoader() {

}

}

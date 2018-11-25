#include "AnsysMeshLoader.h"

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <strstream>
#include <iostream>
#include <iterator>
#include <memory>

#include "Logger.h"
#include "Mesh.h"

namespace mesh {

using namespace std;

AnsysMeshLoader::AnsysMeshLoader(string folder) {
	ifstream inputX((folder + "/x_data.dat").c_str(), ios::in);
	ifstream inputY((folder + "/y_data.dat").c_str(), ios::in);
	ifstream inputZ((folder + "/z_data.dat").c_str(), ios::in);
	ifstream inputNodesFilter((folder + "/nodes.dat").c_str(), ios::in);
	ifstream inputNodeId((folder + "/n_data.dat").c_str(), ios::in);
	ifstream inputTriangles((folder + "/tri.dat").c_str(), ios::in);
	ifstream inputElements((folder + "/elems.dat").c_str(), ios::in);

	if (!(inputX.good() && inputY.good() && inputZ.good()
			&& inputNodesFilter.good() && inputTriangles.good()
			&& inputNodeId.good() && inputElements.good())) {
		class FileNotFoundException: public exception {
			virtual const char* what() const throw () {
				return "Files missing in specified Ansys folder";
			}
		} fileNotFoundException;
		throw fileNotFoundException;
	}

	QSharedPointer<Mesh> qpointer(new Mesh());
	this->mesh = qpointer;
	int nextFilteredNode = -1;
	int i = 1;

	while (true) {
		string lineX;
		string lineY;
		string lineZ;
		string lineNodeId;
		string lineNodesFilter;

		getline(inputX, lineX);
		getline(inputY, lineY);
		getline(inputZ, lineZ);
		getline(inputNodeId, lineNodeId);

		if (inputX.eof() || inputY.eof() || inputZ.eof() || inputNodeId.eof())
			break;

		istringstream stream;
		stream.str(lineX);
		double x;
		stream >> x;
		stream.clear();
		stream.str(lineY);
		double y;
		stream >> y;
		stream.clear();
		stream.str(lineZ);
		double z;
		stream >> z;

		stream.clear();
		stream.str(lineNodeId);
		int nodeId;
		stream >> nodeId;

		Mesh::Point rawPoint;
		rawPoint.x = x;
		rawPoint.y = y;
		rawPoint.z = z;
		mesh->pointVector.push_back(rawPoint);
		mesh->nodeIds.push_back(nodeId);

		if (i == nextFilteredNode || nextFilteredNode < 0) {
			if (nextFilteredNode > 0) {
				mesh->surfaceNodeIdVector.push_back(i);
			}
			getline(inputNodesFilter, lineNodesFilter);
			stream.clear();
			stream.str(lineNodesFilter);
			stream >> nextFilteredNode;
		}
		i++;

	}

	while (true) {
		string triangleLine;

		getline(inputTriangles, triangleLine);
		if (inputTriangles.eof())
			break;

		vector<string> tokens;
		istrstream iss(triangleLine.c_str());
		copy(istream_iterator<string> (iss), istream_iterator<string> (),
				back_inserter<vector<string> > (tokens));

		if (tokens.size() != 3) {
			continue;
		}

		Mesh::Triangle triangle;
		for (unsigned i = 0; i < tokens.size(); i++) {
			istringstream stream;
			stream.str(tokens[i]);
			int x;
			stream >> x;

			if (i == 0) {
				triangle.a = x;
			} else if (i == 1) {
				triangle.b = x;
			} else if (i == 2) {
				triangle.c = x;
			}
		}
		mesh->triangleVector.push_back(triangle);
	}

	while (true) {
		string elementLine;

		getline(inputElements, elementLine);
		if (inputElements.eof())
			break;

		vector<string> tokens;
		istrstream iss(elementLine.c_str());
		copy(istream_iterator<string> (iss), istream_iterator<string> (),
				back_inserter<vector<string> > (tokens));

		if (tokens.size() != 11) {
			continue;
		}

		Mesh::Element element;
		int elementId;

		for (unsigned i = 0; i < tokens.size(); i++) {
			istringstream stream;
			stream.str(tokens[i]);
			int value;
			stream >> value;
			if (i == 0) {
				// Element Id
				elementId = value;
			} else if (i <= 6) {
				// Intermediate node
				element.intermediateNodes.push_back(value);
			} else {
				// Tetrahedron edge
				element.triangleNodes.push_back(value);
			}
		}

		mesh->elements.insert(make_pair(elementId, element));
	}

	inputX.close();
	inputY.close();
	inputZ.close();
	inputNodesFilter.close();
	inputNodeId.close();
	inputTriangles.close();
	inputElements.close();
}

AnsysMeshLoader::~AnsysMeshLoader() {

}

}

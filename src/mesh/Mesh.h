#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <map>
#include <utility>

/**
 * Contains classes for mesh import and export
 */
namespace mesh {

using namespace std;

/**
 * This class holds mesh data
 */
class Mesh {
public:
	friend class AnsysMeshLoader;
	friend class AbaqusMeshLoader;
	/**
	 * A surface triangle of three node Ids
	 */
	struct Triangle {
		int a;
		int b;
		int c;
	};
	/**
	 * A point in space
	 */
	struct Point {
		double x;
		double y;
		double z;
	};
	/**
	 * An element with its node Ids
	 */
	struct Element {
		vector<int> triangleNodes;
		vector<int> intermediateNodes;
	};

protected:
	/**
	 * The surface node Ids
	 */
	vector<int> surfaceNodeIdVector;
	/**
	 * The surface triangles
	 */
	vector<Mesh::Triangle> triangleVector;
	/**
	 * The node Ids
	 */
	vector<int> nodeIds;
	/**
	 * The node coordinates
	 */
	vector<Mesh::Point> pointVector;
	/**
	 * A map that maps the element Ids to the mesh::Mesh::Element data
	 */
	map<int, Element> elements;
public:
	virtual ~Mesh();

	/**
	 * @return The node Ids
	 */
	vector<int>* getNodeIds();
	/**
	 * @return The surface triangles
	 */
	vector<Mesh::Triangle>* getTriangleVector();
	/**
	 * @return The node coordinates
	 */
	vector<Mesh::Point>* getPointVector();
	/**
	 * @return The map that maps the element Ids to the mesh::Mesh::Element data
	 */
	map<int, Element>* getElements();
};

}

#endif /* MESH_H_ */

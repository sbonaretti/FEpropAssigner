#include "MeshLoaderTest.h"

#include "AbaqusMeshLoader.h"
#include "AnsysMeshLoader.h"

namespace test {

MeshLoaderTest::MeshLoaderTest() {
	// TODO Auto-generated constructor stub

}

MeshLoaderTest::~MeshLoaderTest() {
	// TODO Auto-generated destructor stub
}

char* MeshLoaderTest::run() {
	using namespace mesh;
	try {
		AnsysMeshLoader b("./testData/MeshData");
		AbaqusMeshLoader a("./testData/bone1/NetGenMeshAbaqusFormat.inp");
	} catch (exception& e) {
		return "Couldn't load meshes correctly";
	}
	return 0;
}

}

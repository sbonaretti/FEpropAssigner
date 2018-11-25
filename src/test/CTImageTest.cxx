#include "CTImageTest.h"

#include "CTImageMock.h"

namespace test {

char * CTImageTest::run() {
	CTImageMock ctImageMock;

	/*
	// Test getters
	char* getterError = "Error in getter";
	ASSERT(getterError, 20 == ctImageMock.getNx());
	ASSERT(getterError, 10 == ctImageMock.getNy());
	ASSERT(getterError, 5 == ctImageMock.getNz());
	ASSERT(getterError, 5 == ctImageMock.getSx());
	ASSERT(getterError, 3 == ctImageMock.getSy());
	ASSERT(getterError, 0.5 == ctImageMock.getSz());
	ASSERT(getterError, 100 == ctImageMock.getX0());
	ASSERT(getterError, 200 == ctImageMock.getY0());
	ASSERT(getterError, 2 == ctImageMock.getZ0());
	ASSERT(getterError, 20*10*5 == ctImageMock.getNsum());
*/
	return 0;
}

CTImageTest::CTImageTest() {

}

CTImageTest::~CTImageTest() {

}

}

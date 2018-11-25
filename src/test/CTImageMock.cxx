#include "CTImageMock.h"

namespace test {

using namespace ctimage;

CTImageMock::CTImageMock()  {
	/* Don't change these values
	 size[0] = 20;
	 size[1] = 10;
	 size[2] = 5;
	 spacing[0] = 5;
	 spacing[1] = 3;
	 spacing[2] = 0.5;
	 origin[0] = 100;
	 origin[1] = 200;
	 origin[2] = 2;
	 */

	size[0] = 20;
	size[1] = 10;
	size[2] = 5;
	spacing[0] = 5;
	spacing[1] = 3;
	spacing[2] = 0.5;
	origin[0] = 100;
	origin[1] = 200;
	origin[2] = 2;

	int n = size[0] * size[1] * size[2];


	CTImage::CTPixelType* pixelData = new CTImage::CTPixelType[n];
	for (int i = 0; i < n; i++) {
		pixelData[i] = i;
	}

	this->pixelData = pixelData;
}

CTImageMock::~CTImageMock() {

}

}

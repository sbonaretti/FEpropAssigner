#ifndef CTIMAGEMOCK_H_
#define CTIMAGEMOCK_H_

#include "CTImage.h"

namespace test {

using namespace ctimage;

class CTImageMock: public CTImage {
public:
	CTImageMock();
	virtual ~CTImageMock();
};

}

#endif /* CTIMAGEMOCK_H_ */

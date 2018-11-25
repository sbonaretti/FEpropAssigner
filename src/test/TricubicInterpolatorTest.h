#ifndef TRICUBICINTERPOLATORTEST_H_
#define TRICUBICINTERPOLATORTEST_H_

#include "TricubicInterpolator.h"
#include "Test.h"

namespace test {

class TricubicInterpolatorTest: public Test {
public:
	TricubicInterpolatorTest();
	char* run();
	virtual ~TricubicInterpolatorTest();
};

}

#endif /* TRICUBICINTERPOLATORTEST_H_ */

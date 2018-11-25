#ifndef FEMPROPERTYASSIGNERTEST_H_
#define FEMPROPERTYASSIGNERTEST_H_

#include "Test.h"

namespace test {

class FEMPropertyAssignerTest: public Test {
public:
	FEMPropertyAssignerTest();
	char* run();
	virtual ~FEMPropertyAssignerTest();
};

}

#endif /* FEMPROPERTYASSIGNERTEST_H_ */

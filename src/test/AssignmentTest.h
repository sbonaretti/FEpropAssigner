#ifndef ASSIGNMENTTEST_H_
#define ASSIGNMENTTEST_H_

#include "Test.h"

namespace test {

class AssignmentTest : public Test {
public:
	AssignmentTest();
	char* run();
	virtual ~AssignmentTest();
};

}

#endif /* ASSIGNMENTTEST_H_ */

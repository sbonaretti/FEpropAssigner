#ifndef TEST_H_
#define TEST_H_

#include <iostream>
#include <typeinfo>


using namespace std;

/**
 * Contains unit tests
 */
namespace test {
#define RUN(test) do { test t; cout << "Running: " << typeid(t).name() << "\n";\
	char* message = t.run(); if (message) return message; } while (0)
#define ASSERT(message,test) do { if (!(test)) return message; } while (0)
#define FAIL(message) do { return message; } while (0)

/**
 * Abstract Test class
 */
class Test {
public:
	virtual char* run() = 0;
};

}

#endif /* TEST_H_ */

#define LIE_TEST_RUNNER
#pragma once

#include <vector>
#include "HeterogenObject.h"

namespace LIE{
class  Test{
private:
	static std::vector<HOB<Test>> tests;
protected:
	virtual void RunTest () const = 0;
public:
	static void Run();
	static void Add (const HOB<Test>& test);
};

}

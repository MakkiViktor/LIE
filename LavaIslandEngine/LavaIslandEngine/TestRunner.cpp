#include "TestRunner.h"

namespace LIE{
std::vector<HOB<Test>> Test::tests = std::vector<HOB<Test>>();
}

void LIE::Test::Run (){
	for(const HOB<Test>& test : tests){
		test()->RunTest ();
	}
}

void LIE::Test::Add (const HOB<Test>& test){
	tests.push_back (test);
}

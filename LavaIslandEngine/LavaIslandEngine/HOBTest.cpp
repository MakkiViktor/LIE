#define HOB_TEST
#pragma

#include "TestRunner.h"
#include <iostream>


namespace{

class HOBTestParent{
public:
	virtual void print () const{
		std::cout << "Parent";
	}
};

class HOBTestChild : public HOBTestParent{
public:
	virtual void print () const override{
		std::cout << "Child";
	}
};

}

namespace LIE{

class HOBTest : public LIE::Test{
	virtual void RunTest () const override{
		HOB<HOBTestParent> obj1{HOBTestParent()};
		HOB<HOBTestParent> obj2{HOBTestChild()};
		obj1 ()->print ();
		obj2 ()->print ();
	}
public:
	void Run (){
		RunTest ();
	}
};

}

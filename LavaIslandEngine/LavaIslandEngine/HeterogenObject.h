#define LIE_HETEROGEN_OBJECT
#pragma once

#include "Types.h"

namespace LIE{

template<class PARENT>
class HOB{
private:
	PARENT* object;
	bool isCopy;
public:
	HOB () = delete;
	template<class CHILD>
	HOB (const CHILD& object);
	~HOB ();
	HOB (const HOB& other);
	HOB (HOB&& other);
	PARENT * const operator()() const;
};

template<class PARENT>
template<class CHILD>
inline HOB<PARENT>::HOB (const CHILD & object):
	object{new CHILD(object)},
	isCopy(false)
{
}

template<class PARENT>
inline HOB<PARENT>::~HOB (){
	if(!isCopy)
		delete object;
}

template<class PARENT>
inline HOB<PARENT>::HOB (const HOB & other):
	object(other.object),
	isCopy(false)
{
	const_cast<HOB*>(&other)->isCopy = true;
}

template<class PARENT>
inline HOB<PARENT>::HOB (HOB && other):
	object(other.object),
	isCopy(other.isCopy)
{
	other.object = nullptr;
}

template<class PARENT>
inline PARENT * const HOB<PARENT>::operator()() const{
	if(object == nullptr){
		ERROR ("object was deleted");
	}
	return object;
}

}

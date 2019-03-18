#include "CopyInfo.h"

SO::CopyInfo::CopyInfo (const CopyInfo &){
	isCopy = true;
}

SO::CopyInfo::CopyInfo (CopyInfo && other){
	isCopy = other.isCopy;
}

void SO::CopyInfo::operator=(CopyInfo && other){
	isCopy = other.isCopy;
}

void SO::CopyInfo::operator=(const CopyInfo &){
	isCopy = true;
}

bool SO::CopyInfo::IsCopy () const{
	return isCopy;
}

#ifndef LIE_COPY_INFO
#define LIE_COPY_INFO
#pragma once

namespace SO{
class CopyInfo{
private:
	bool isCopy = false;
public:
	CopyInfo(){}
	CopyInfo (const CopyInfo&);
	CopyInfo (CopyInfo&& other);
	void operator=(CopyInfo&& other);
	void operator=(const CopyInfo&);

	bool IsCopy () const;
};
}

#endif // !LIE_COPY_INFO

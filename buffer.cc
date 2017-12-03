#define _Buffer

#ifndef _GListChars
#include "../MyLibCC/DataStruct/gListChars.cc"
#endif

class Buffer : public GListChars{
public:
	Buffer()
		:GListChars(){
	}

	void write(uint posX, uchar c){
		getChars()[posX]=c;
	}

	uchar getC(uint posX){
		return getChars()[posX];
	}
};

#define _Buffer

#ifndef _GListChars
#define _GListCharsMain
#include "../MyLibCC/DataStruct/gListChars.cc"
#undef _GListCharsMain
#endif

class Buffer : public GListChars{
public:
	Buffer()
		:GListChars(){
	}

	void addAfter(){
		GListChars::addAfter(32);
	}

	void write(uint posX, uchar c){
		GListFrameChars *f=getFrame();
		if(c==0 && (*f).getLength()>32 && posX<(*f).getLength()-48){
			(*f).resize_add(-32);
		}else if(posX > (*f).getLength()-8){
			(*f).resize_add(32);
		}
		
		(*(*f)[posX])=c;
	}

	uchar getC(uint posX){
		return (*(*getFrame())[posX]);
	}
};

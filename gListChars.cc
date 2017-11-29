#define _GListChars

#ifndef _GList
#include "../MyLibCC/DataStruct/gList.cc"
#endif

class GListFrameChars : public GListFrame{
public:
	uchar *c;

	GListFrameChars(uint i)
		:GListFrame(){
		c=(uchar*)calloc(i, sizeof(char));
		if(c==NULL){
			Tool_error0MemoryAllocation("GListFrameChars(...)");
		}
	}

	~GListFrameChars(){
		free(c);
	}
};

class GListChars : public GList{
public:
	GListChars()
		:GList(){
	}

	void addAfter(uint i){
		GList::addAfter(new GListFrameChars(i));
	}

	void addBefore(uint i){
		GList::addBefore(new GListFrameChars(i));
	}

	GListFrameChars* getFrame(){
		return (GListFrameChars*)GList::getFrame();
	}

	uchar* getChars(){
		return (*GListChars::getFrame()).c;
	}
};

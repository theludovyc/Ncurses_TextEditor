#define _Cursor

#ifndef _Vec
#include "../MyLibCC/Geom/vec.cc"
#endif

class Cursor : public Vec{
private:
	void move0(Buffer *buffer){
		if( (*buffer).getC(0) == 0){
		  x=0;
		}else if( x>0 ){
			while( (*buffer).getC( x-1 ) == 0){
			  x--;
			}
		}
	}
	
public:
	Cursor()
		:Vec(){
	}

	void moveX(WINDOW *w, uint posX){
		x=posX;
		wmove(w, y, posX);
	}

	void moveY(WINDOW *w, uint posY){
	  y=posY;
		wmove(w, posY, x);
	}

	void reset(WINDOW *w){
		wmove(w, y, x);
	}

	void moveUp(WINDOW *w, Buffer *buffer){
		(*buffer).rollBefore();
	  y--;

	  move0(buffer);
	
	  reset(w);
	}

	void moveDown(WINDOW *w, Buffer *buffer){
	  (*buffer).rollAfter();
	  y++;

	  move0(buffer);
	
	  reset(w);
	}

	void moveLeft(WINDOW *w){
	  x--;
	  reset(w);
	}

	void moveRight(WINDOW *w){
	  x++;
	  reset(w);
	}

	void moveXY(WINDOW *w, uint posX, uint posY){
	  x=posX;
	  y=posY;
		reset(w);
	}
};

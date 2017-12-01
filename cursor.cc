#define _Cursor

#ifndef _Vec
#include "../MyLibCC/Geom/vec.cc"
#endif

class Cursor : public Vec{
public:
	Cursor()
		:Vec(){
	}

	void moveX(uint posX){
		x=posX;
		move( y, posX);
	}

	void moveY(uint posY){
	  y=posY;
		move( posY, x);
	}

	void reset(){
		move(y, x);
	}

	void moveUp(Buffer *buffer){
		(*buffer).rollBefore();
	  y--;

		if( (*buffer).getC(0) == 0){
		  x=0;
		}
	
	  reset();
	}

	void moveDown(Buffer *buffer){
		(*buffer).rollAfter();
	  y++;

		if( (*buffer).getC(0) == 0){
		  x=0;
		}
	
	  reset();
	}

	void moveLeft(){
	  x--;
	  reset();
	}

	void moveRight(){
	  x++;
	  reset();
	}

	void moveXY(uint posX, uint posY){
	  x=posX;
	  y=posY;
		reset();
	}
};

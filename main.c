#define _Main

#include "../MyLibCC/Tool.cc"
#include "../MyLibCC/Geom/vec.cc"

#include <ncurses.h>

#include "gListChars.cc"

unsigned char again=1;

int key;

Vec *cursor;

unsigned int screen_width;
unsigned int screen_height;

GListChars *buffer;

void buffer_write(uint posX, uchar c){
	(*buffer).getChars()[posX]=c;
}

void buffer_init(){
	buffer=new GListChars();

	(*buffer).addAfter(screen_width);

	/*
	int i;
	for(i=0; i<screen_height;i++){
		buffer_write(0, i, '\0');
	}
	*/
}

void buffer_write(uchar c){
  buffer_write( (*cursor).x, c);
}

uchar buffer_getc(uint posX){
	return (*buffer).getChars()[posX];
}

uchar buffer_isER(uint posX){
	uchar c=buffer_getc(posX);
	if(c==0 || c=='\n'){
		return 1;
	}
	return 0;
}

void buffer_save(){
	FILE *fp;
	fp=fopen("test.c","w");

	if( (*cursor).y!=0){
		(*buffer).rollBefore((*cursor).y);
	}
		
	uint i,j;
	uchar c;

	for(j=0; j<(*buffer).getLength(); j++){
		i=0;
	  while(true){
			c=buffer_getc(i);
			if(c==0){
				fputc('\n', fp);
				(*buffer).rollAfter();
				break;
			}

			fputc(c, fp);
			i++;
		}
	}
	
	fclose(fp);
}

void cursor_moveX(uint posX){
	(*cursor).x=posX;
	move( (*cursor).y, posX);
}

void cursor_move(uint posY){
	(*cursor).y=posY;
	move( posY, (*cursor).x);
}

void cursor_reset(){
	move( (*cursor).y, (*cursor).x);
}

void cursor_moveUp(){
	(*buffer).rollBefore();
	(*cursor).y--;

	if( buffer_getc(0) == 0){
		(*cursor).x=0;
	}
	
	cursor_reset();
}

void cursor_moveDown(){
	(*buffer).rollAfter();
	(*cursor).y++;

	if( buffer_getc(0) == 0){
		(*cursor).x=0;
	}
	
	cursor_reset();
}

void cursor_moveLeft(){
	(*cursor).x--;
	cursor_reset();
}

void cursor_moveRight(){
	(*cursor).x++;
	cursor_reset();
}

void cursor_move(uint posX, uint posY){
	(*cursor).x=posX;
	(*cursor).y=posY;
	move(posY, posX);
}

int main(){
	cursor=new Vec(0, 0);
	
	initscr();

	keypad(stdscr, TRUE);
	noecho();

	getmaxyx(stdscr, screen_height, screen_width);

	buffer_init();
	
	while(again){
		switch(key=getch()){
		case KEY_F(2):
			again=0;
			break;

		case KEY_F(3):
			buffer_save();
			break;

		case 9:
			buffer_write('\t');
			addch('#');
			(*cursor).x++;
			break;

			//enter
		case 10:
			(*buffer).addAfter(screen_width);
		  cursor_move(0, (*cursor).y+1);
			break;

		case KEY_BACKSPACE:
			if( (*cursor).x!=0){
				cursor_moveLeft();
				buffer_write(0);
				addch(' ');
				cursor_reset();
			}
			break;
			
		case KEY_UP:
			if( (*cursor).y!=0){
				cursor_moveUp();
			}
			break;

		case KEY_DOWN:
			if((*cursor).y<(*buffer).getLength()-1 && (*cursor).y<screen_height){
				cursor_moveDown();
			}
			break;

		case KEY_LEFT:
			if( (*cursor).x!=0){
				cursor_moveX( (*cursor).x-1);
			}
			break;

		case KEY_RIGHT:
			if( !buffer_isER( (*cursor).x) ){
				cursor_moveRight();
			}
			break;
			
		default:
		  buffer_write(key);
			addch(key);
			(*cursor).x++;
		}

		refresh();
	}
	
	delete(cursor);
	delete(buffer);
	
	endwin();

	return 0;
}

#define _Main

#include "../MyLibCC/Tool.cc"

#include <ncurses.h>

#include "buffer.cc"
#include "cursor.cc"

unsigned char again=1;

int key;

unsigned int screen_width;
unsigned int screen_height;

Buffer *buffer;
Cursor *cursor;

void buffer_init(){
	buffer=new Buffer();

	(*buffer).addAfter(screen_width);

	/*
	int i;
	for(i=0; i<screen_height;i++){
		buffer_write(0, i, '\0');
	}
	*/
}

uchar buffer_getC(){
	return (*buffer).getC((*cursor).x);
}

void buffer_writeAfter(uchar c){
	uchar charIn=c;
	uchar charSave;
	uint posX=(*cursor).x;

	do{
		charSave=(*buffer).getC(posX);

		(*buffer).write(posX, charIn);
		addch(charIn);

		charIn=charSave;

		posX++;
	}while(charSave!=0);

	(*cursor).x++;
	(*cursor).reset();
}

void buffer_write(uchar c){
	(*buffer).write((*cursor).x, c);
}

void buffer_rem(){
	if( (*cursor).x!=0){
		if(buffer_getC() == 0){
			(*buffer).write((*cursor).x-1, 0);
			mvaddch((*cursor).y, (*cursor).x-1, ' ');
			(*cursor).moveLeft();
			return;
		}

		(*cursor).moveLeft();
		uint posX=(*cursor).x;
		uint c;
		
		do{
			c=(*buffer).getC(posX+1);
			(*buffer).write(posX, c);
			
			if(c==0){
				addch(' ');
				break;
			}else{
				addch(c);
			}
			
			posX++;
		}while(true);

		(*cursor).reset();
	}
}

uchar buffer_isER(uint posX){
	uchar c=(*buffer).getC(posX);
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
			c=(*buffer).getC(i);
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

void cursor_moveUp(){
	(*cursor).moveUp(buffer);
}

void cursor_moveDown(){
	(*cursor).moveDown(buffer);
}

int main(){
	cursor=new Cursor();
	
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
		  (*cursor).moveXY(0, (*cursor).y+1);
			break;

		case KEY_BACKSPACE:
			buffer_rem();
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
				(*cursor).moveLeft();
			}
			break;

		case KEY_RIGHT:
			if( !buffer_isER( (*cursor).x) ){
				(*cursor).moveRight();
			}
			break;
			
		default:
		  buffer_writeAfter(key);
			//addch(key);
			//(*cursor).x++;
		}

		refresh();
	}
	
	delete(cursor);
	delete(buffer);
	
	endwin();

	return 0;
}

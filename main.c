#define _Main

#include "../MyLibCC/tool.cc"

#include <ncurses.h>

#include "bufferWin.cc"

BufferWin *bufferWin;

unsigned char again=1;

int key;

int main(){
	initscr();

	raw();
	noecho();
	keypad(stdscr, TRUE);

	bufferWin=new BufferWin(stdscr, LINES, COLS, 0, 0);
	
	while(again){
		switch(key=getch()){
		case KEY_F(2):
			again=0;
			break;

		case KEY_F(3):
			(*bufferWin).save();
			break;

			/*case 9:
			buffer_write('\t');
			addch('#');
			(*cursor).x++;
			break;*/

		case KEY_F(4):
			(*bufferWin).moveXY(3, 0);
			break;
			
		default:
		  (*bufferWin).onKey(key);
		}

		(*bufferWin).refresh(stdscr);
	}

	delete(bufferWin);
	
	endwin();

	return 0;
}

#define _Main

#include "../MyLibCC/Tool.cc"
#include "../MyLibCC/Geom/vec.cc"

#include <ncurses.h>

unsigned char again=1;

int key;

Vec *cursor;

unsigned int screen_width;
unsigned int screen_height;

unsigned char *buffer;

void buffer_write(unsigned int posX, unsigned int posY, unsigned char c){
	buffer[screen_width*posY+posX]=c;
}

void buffer_init(){
	int i;
	for(i=0; i<screen_height;i++){
		buffer_write(0, i, '\0');
	}
}

void buffer_write(unsigned char c){
  buffer_write( (*cursor).x, (*cursor).y, c);
}

unsigned char buffer_getc(unsigned int posX, unsigned int posY){
	return buffer[screen_width*posY+posX];
}

uchar buffer_isER(uint posX, uint posY){
	uchar c=buffer_getc(posX, posY);
	if(c==0 || c=='\n'){
		return 1;
	}
	return 0;
}

void buffer_save(){
	FILE *fp;
	fp=fopen("test.c","w");

	uint i,j;
	for(j=0; j<screen_height; j++){
		for(i=0; i<screen_width; i++){
		  if(buffer_getc(i, j)==0){
				if(i!=0){
					fputc('\n', fp);
				}
				break;
			}

			fputc(buffer_getc(i, j), fp);
		}
	}
	
	fclose(fp);
}

void cursor_moveX(uint posX){
	(*cursor).x=posX;
	move( (*cursor).y, posX);
}

void cursor_moveY(uint posY){
	(*cursor).y=posY;
	move( posY, (*cursor).x);
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
	
	buffer=(unsigned char*)calloc(screen_width*screen_height, sizeof(char));
	if(buffer==NULL){
		Tool_error0MemoryAllocation("Buffer malloc");
	}

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
		  cursor_move(0, (*cursor).y+1);
			break;

		case KEY_BACKSPACE:
			if( (*cursor).x!=0){
				move((*cursor).y, (*cursor).x-1);
				addch(' ');
				(*cursor).x--;
				move((*cursor).y, (*cursor).x);
			}
			break;
			
		case KEY_UP:
			if( (*cursor).y!=0){
				cursor_moveY( (*cursor).y-1);
			}
			break;

		case KEY_DOWN:
			break;

		case KEY_LEFT:
			if( (*cursor).x!=0){
				cursor_moveX( (*cursor).x-1);
			}
			break;

		case KEY_RIGHT:
			if( !buffer_isER( (*cursor).x, (*cursor).y)){
				cursor_moveX( (*cursor).x+1);
			}
			break;
			
		default:
		  buffer_write(key);
			addch(key);
			(*cursor).x++;
			move((*cursor).y, (*cursor).x);
		}

		refresh();
	}
		
	endwin();

	return 0;
}

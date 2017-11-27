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

void buffer_save(){
	FILE *fp;
	fp=fopen("test.c","w");

	uint i,j;
	for(j=0; j<screen_height; j++){
		for(i=0; i<screen_width; i++){
		  if(i==0 && buffer_getc(0, j)=='\0'){
				break;
			}

			if(buffer_getc(i, j)=='\n'){
				fputc('\n', fp);
				break;
			}

			fputc(buffer_getc(i, j), fp);
		}
	}
	
	fputc('\0',fp);
	
	fclose(fp);
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
		case KEY_DOWN:
			again=0;
			break;

		case 10:
		  buffer_write('\n');
			(*cursor).y++;
			(*cursor).x=0;
			move((*cursor).y, 0);
			break;

		case KEY_UP:
			buffer_save();
			break;

		case KEY_BACKSPACE:
			(*cursor).x--;
			move((*cursor).y, (*cursor).x);
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

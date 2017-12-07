#define _BufferWin

#include "buffer.cc"
#include "cursor.cc"

class BufferWin{
private:
	WINDOW *win;
	WINDOW *win_line;
	Cursor *cursor;
	Buffer *buffer;

	void buffer_write(uchar c){
		(*buffer).write((*cursor).x, c);
	}

	void buffer_writeAfter(uchar c){
		uchar charIn=c;
		uchar charSave;
		uint posX=(*cursor).x;

		do{
			charSave=(*buffer).getC(posX);

			(*buffer).write(posX, charIn);
			waddch(win, charIn);

			charIn=charSave;

			posX++;
		}while(charSave!=0);

		(*cursor).x++;
		(*cursor).reset(win);
	}

	void buffer_addLine(){
		(*buffer).addAfter();
		(*cursor).moveXY(win, 0, (*cursor).y+1);
		mvwprintw(win_line, (*cursor).y, 0, "%d", (*cursor).y);
		touchwin(win_line);
		wrefresh(win_line);
	}

	uchar buffer_getC(){
		return (*buffer).getC((*cursor).x);
	}

	void buffer_rem(){
		if( (*cursor).x!=0){
			if(buffer_getC() == 0){
				(*buffer).write((*cursor).x-1, 0);
				mvwaddch(win, (*cursor).y, (*cursor).x-1, ' ');
				(*cursor).moveLeft(win);
				return;
			}

			(*cursor).moveLeft(win);
			uint posX=(*cursor).x;
			uint c;
		
			do{
				c=(*buffer).getC(posX+1);
				(*buffer).write(posX, c);
			
				if(c==0){
					waddch(win, ' ');
					break;
				}else{
					waddch(win, c);
				}
			
				posX++;
			}while(true);

			(*cursor).reset(win);
		}
	}

	uchar buffer_isER(uint posX){
		uchar c=(*buffer).getC(posX);
		if(c==0 || c=='\n'){
			return 1;
		}
		return 0;
	}	

	void cursor_moveUp(){
		(*cursor).moveUp(win, buffer);
	}

	void cursor_moveDown(){
		(*cursor).moveDown(win, buffer);
	}

	void cursor_moveLeft(){
		(*cursor).moveLeft(win);
	}

	void cursor_moveRight(){
		(*cursor).moveRight(win);
	}
	
public:
	BufferWin(WINDOW *orig, int nlines, int ncols, int posX, int posY){
		win=derwin(orig, nlines, ncols-2, posY, posX+2);
		win_line=derwin(orig, nlines, 2, 0, 0);

		cursor=new Cursor();
		buffer=new Buffer();

		(*buffer).addAfter();
		waddch(win_line, '0');
		touchwin(orig);
		wrefresh(win_line);
	}

	~BufferWin(){
		delwin(win);
		delete(cursor);
		delete(buffer);
	}

	void save(){
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

	void onKey(int key){
		switch(key){
		case 10: //enter
			buffer_addLine();
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
			if((*cursor).y<(*buffer).getLength()-1 && (*cursor).y<LINES){
				cursor_moveDown();
			}
			break;

		case KEY_LEFT:
			if( (*cursor).x!=0){
				cursor_moveLeft();
			}
			break;

		case KEY_RIGHT:
			if( !buffer_isER( (*cursor).x) ){
				cursor_moveRight();
			}
			break;
			
		default:
			buffer_writeAfter(key);
		}
	}

	void refresh(WINDOW *orig){
		touchwin(win);
		wrefresh(win);
	}

	void moveXY(int posX, int posY){
		mvwin(win, posY, posX);
	}
};

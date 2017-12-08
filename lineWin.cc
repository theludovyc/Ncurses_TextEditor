#define _LineWin

class LineWin{
private:
	WINDOW *win;
	uint cols;

	void calcCols(uint nlines){
		if(nlines < 10){
			cols=2;
			return;
		}else if(nlines < 100){
			cols=3;
			return;
		}else if(nlines < 1000){
			cols=4;
			return;
		}
		cols=5;
		return;
	}

public:
	LineWin(WINDOW *orig, uint nlines, int posX, int posY){
		calcCols(nlines);
		
		win=derwin(orig, nlines, cols, posY, posX);
	}

	~LineWin(){
		delwin(win);
	}

	uint getCols(){
		return cols;
	}

	void refresh(uint startLine, uint endLine){
		uint i;
		switch(cols){
		case 2:
			for(i=0; startLine+i<endLine; i++){
				mvwprintw(win, i, 0, "%d", startLine+i);
			}
			break;

		case 3:
			for(i=0; startLine+i<endLine; i++){
				mvwprintw(win, i, 0, "%2d", startLine+i);
			}
			break;

		case 4:
			for(i=0; startLine+i<endLine; i++){
				mvwprintw(win, i, 0, "%3d", startLine+i);
			}
			break;

		case 5:
			for(i=0; startLine+i<endLine; i++){
				mvwprintw(win, i, 0, "%4d", startLine+i);
			}
			break;
		}
		
		touchwin(win);
		wrefresh(win);
	}
};

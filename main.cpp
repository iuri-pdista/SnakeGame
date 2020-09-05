#include <stdlib.h> 
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <term.h>

int mapWidth = 40;
int mapHeight = 20;

typedef struct fruit {
	int x;
	int y;
	int WasEaten;
} Fruit;

typedef struct character {
	int x;
	int y;
	int score;
} Character;

void RenderMap () {
	int x = 0;
	do {
		for ( int y = 0; y < mapHeight; y++ ){
			move( y, x );
			if ( y == 0 || y == 19 ){
				printw("#");
				refresh();
			}
			else{
				if( x == 0 || x == 39){
					printw("#");
					refresh();
				}
				else{
					printw(" ");
					refresh();
				}
			}
		}
		printw("\n");
		x += 1;
	}while(x < mapWidth);
}

int main (){
	initscr();
	noecho();
	system("clear");
	RenderMap();
	while (1){
		getch();
	}
	endwin();
}

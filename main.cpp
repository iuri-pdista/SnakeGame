#include <stdlib.h> 
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <term.h>

int mapWidth = 40;
int mapHeight = 20;
int GameOver = 0;

typedef struct fruit {
	int x;
	int y;
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

int RandInt ( int max ) {
	srand(time(NULL));
	int rndNum = 0;
	rndNum = rand() % max;
	return rndNum + 1;
}

Fruit* GenerateFruit () {
	Fruit* NewFruit = (Fruit*) malloc(sizeof(Fruit));
	(*NewFruit).x = 0;
	(*NewFruit).x = RandInt(mapWidth - 1);
	(*NewFruit).y = 0;
	(*NewFruit).y = RandInt(mapHeight - 1);
	printw("%d %d", (*NewFruit).x, (*NewFruit).y);
	refresh();
	move( (*NewFruit).y , (*NewFruit).x );
	printw("X");
	refresh();
	return NewFruit;
}

int main (){
	initscr();
	noecho();
	system("clear");
	RenderMap();
	move(0,0);
	GenerateFruit();
	while (1){
		getch();
	}
	endwin();
}

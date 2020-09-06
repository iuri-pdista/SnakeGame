#include <stdlib.h> 
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <term.h>

int mapWidth = 40;
int mapHeight = 20;
int GameOver = 0;
int Score = 0;

typedef struct fruit {
	int x;
	int y;
} Fruit;

typedef struct character {
	int x;
	int y;
	int score;
} Character;

void PrintScore(){
	move(0,0);
	printw("%d", Score);
}

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
	move( (*NewFruit).y , (*NewFruit).x );
	printw("X");
	refresh();
	return NewFruit;
}

void EraseMove(Character* Avatar) {
	move((*Avatar).y, (*Avatar).x );
	printw(" ");
	refresh();
}

Character* InitializeCharacter ( int score ){
	Character* Hero = (Character*) malloc (sizeof(Character));
	(*Hero).x = 2;
	(*Hero).y = 2;
	refresh();
	return Hero;
}

Character* MoveCharacter( Character* Avatar ){
	int key = getch();
	switch( key ){
		case 119:
			EraseMove(Avatar);
			(*Avatar).y -= 1;
			move ( (*Avatar).y, (*Avatar).x );
			printw("O");
			refresh();
			return Avatar;
		break;
		case 115:
			EraseMove(Avatar);
			(*Avatar).y += 1;
			move( (*Avatar).y, (*Avatar).x );
			printw("O");
			refresh();
			return Avatar;
		break;
		case 97:
			EraseMove(Avatar);
			(*Avatar).x -= 1;
			move( (*Avatar).y, (*Avatar).x );
			printw("O");
			refresh();
			return Avatar;
		break;
		case 100:
			EraseMove(Avatar);
			(*Avatar).x += 1;
			move( (*Avatar).y, (*Avatar).x );
			printw("O");
			refresh();
			return Avatar;
		break;
		default:
			return Avatar;
	}
}

int ValidateMove( Character* Avatar, Fruit* fruit ){
	int charX = (*Avatar).x;
	int charY = (*Avatar).y;
	int fruitX = (*fruit).x;
	int fruitY = (*fruit).y;
	if ( charX == fruitX && charY == fruitY ){
		Score += 1;
		PrintScore();
		return 1;
	}
	if ( charX == 0 || charX == mapWidth || charY == 0 || charY == mapHeight ){
		move(9,18);
		printw("YOU'VE LOST\n Press W to play again.");
		refresh();
		int Replay = getch();
		if(Replay == 119 )
			GameOver = 1;
		else{
			endwin();
			exit(0);
		}	
	}
	return 0;
}

int GameOn (Character* Hero, Fruit* newFruit){
	while ( GameOver != 1 ){
		MoveCharacter(Hero);
		int theFruitWasEaten = ValidateMove(Hero, newFruit);
		if ( theFruitWasEaten == 1 ){
			Character* Hero = InitializeCharacter( (*Hero).score );
			Fruit* newFruit = GenerateFruit();
			GameOn( Hero, newFruit );
		}
	}
	return 1;
}

int main (){
	initscr();
	noecho();
	RenderMap();
	move(0,0);
	Fruit* newFruit = GenerateFruit();
	Character* Hero = InitializeCharacter(0);
	GameOn(Hero, newFruit);
	GameOver = 0;
	main();
	endwin();
}

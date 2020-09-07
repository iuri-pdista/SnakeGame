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
} Character;

typedef struct enemy {
	int x;
	int y;
	enemy* NxtEnemy;
	enemy* LastEnemy;
} Enemy;

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

int RemoveExceptions( int rndNum, int* exceptions){
	for (int i = 0; i < ( (sizeof(exceptions) ) / (sizeof(int) )); i++){
		if ( rndNum == exceptions[i] ){
			return 1;
		}		
	}
	return 0;
}

int RandInt ( int max ) {
	srand(time(NULL));
	int rndNum = 0;
	rndNum = (rand() % max) + 1;
	return rndNum;
}

int RandIntWithoutExceptions ( int max, int* exceptions ) {
	int rndNum = RandInt(max);
	if ( RemoveExceptions(rndNum, exceptions) == 1 ){
		RandIntWithoutExceptions( max, exceptions );
	}
	return rndNum;
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

Character* InitializeCharacter ( ){
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
		move( (*Avatar).y, (*Avatar).x );
		printw(" ");
		refresh();
		return 1;
	}
	if ( charX == 0 || charX == mapWidth || charY == 0 || charY == mapHeight ){
		move(9,18);
		printw("YOU'VE LOST\n Press W to play again.");
		refresh();
		int Replay = getch();
		if( Replay == 119 ){
			GameOver = 1;
			Score = 0;
		}
		else{
			endwin();
			exit(0);
		}	
	}
	return 0;
}

void PrintEnemy ( Enemy* enemy ){
	for ( Enemy* nxtEnemy = enemy; (*nxtEnemy).LastEnemy != NULL; nxtEnemy = (*nxtEnemy).LastEnemy ){
		if ( (*nxtEnemy).x == 0 && (*nxtEnemy).y == 0 ){
			move(RandInt(mapHeight), RandInt(mapWidth));
			printw("@");
			refresh();
		}
		move ( (*nxtEnemy).y, (*nxtEnemy).x );
		printw("@");
		refresh();
		PrintScore();
	}
}

Enemy* InitializeEnemy ( Enemy* lastEnemy, Enemy* nxtEnemy ){
	Enemy* newEnemy = (Enemy*) malloc(sizeof(Enemy));
	(*lastEnemy).NxtEnemy = newEnemy;
	(*newEnemy).LastEnemy = lastEnemy;
	(*newEnemy).NxtEnemy = nxtEnemy;
	return newEnemy;
}

Enemy* GenerateEnemies (Fruit* fruit){
	int MaxEnemies = RandInt(5);
	Enemy* enemiesInitialPtr = (Enemy*) malloc(sizeof(Enemy));
	Enemy* listPointer = InitializeEnemy( enemiesInitialPtr, NULL );
	(*listPointer).x = 0;
	(*listPointer).y = 0;
	int exceptions[] = { (*fruit).x , (*fruit).x, 2, 2};
	for ( int i = 0; i < MaxEnemies; i++ ){
		listPointer = InitializeEnemy(listPointer, NULL);
		(*listPointer).x = RandIntWithoutExceptions( mapWidth - 2, exceptions );
		(*listPointer).y = RandIntWithoutExceptions( mapHeight - 2, exceptions );
	}
	PrintEnemy( listPointer );
	return enemiesInitialPtr;
}

int GameOn (Character* Hero, Fruit* newFruit){
	GenerateEnemies(newFruit);
	while ( GameOver != 1 ){
		MoveCharacter(Hero);
		int theFruitWasEaten = ValidateMove(Hero, newFruit);
		if ( theFruitWasEaten == 1 ){
			Character* Hero = InitializeCharacter( );
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
	Character* Hero = InitializeCharacter();
	GameOn(Hero, newFruit);
	GameOver = 0;
	main();
	endwin();
}

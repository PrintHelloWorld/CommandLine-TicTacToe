/*
* TICTACTOE -- Human vs. Computer
*
* Authors: Ian Welch & Blake McCawe
* Date: June 2015
*
*/

#include <stdio.h>
#include <stdlib.h>
#include "tictactoe.h"

int check(TicTacToe* gameptr);
void init_game(TicTacToe *gameptr, int size);
int player_move(TicTacToe *gameptr);
void computer_move(TicTacToe* gameptr);
void print_game(TicTacToe game);
char tokenstr(int token);
void print_result(TicTacToe game);
void free_game(TicTacToe *gameptr);


int main(void){
	int size = 0;
	printf("Welcome to TicTacToe.\n");
	printf("You will be playing against the computer.\n");
	printf("Enter the size of the board: ");
	/* if not integer */
	if(scanf("%d", &size) != 1 || size<= 0){
		printf("Size must be of an Integer greater than 0. \n");
		return -1; /* main failed with errors */
	}
	printf("The game board is %d by %d\n", size, size);
	/* create new game */
	TicTacToe gameptr;
	init_game(&gameptr,size);
	int done;
	do{
		print_game(gameptr);
	do {
		done = player_move(&gameptr);
	} while (!done); /* loop until valid move */
	if(check(&gameptr) != FALSE) break; /* was a winner or a draw */
	computer_move(&gameptr);
	if(check(&gameptr) != FALSE) break; /* was a winner or a draw */
	} while(TRUE);
	print_result(gameptr);
	print_game(gameptr);
	free_game(&gameptr);

	/* main success */
	return 0;
}

/*
* Initialises the 2D array with 0 values
* that represents the games board
*/
void init_game(TicTacToe *gameptr, int size){
	int i, j;
	int **board = (int **)malloc(sizeof(int *)*size);
	for (i = 0; i < size; i++)
		board[i] = (int *)malloc(sizeof(int)*size);

	/* intialize board to none */
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			board[i][j] = NONE;

	gameptr->board = board;
	gameptr->size = size;
	gameptr->winner = NONE;
}

/*
* Displays the current state of the game
*/
void print_game(TicTacToe game){
	int i, j;
	for (i = 0; i < game.size; i++) {
		for (j = 0; j < game.size; ++j) {
			printf("%c ",tokenstr(game.board[i][j]));
		}
		printf("\n");
	}
	printf("\n");
}

/*
* Displays the result of the game
*/
void print_result(TicTacToe game){
	if(game.winner==HUMAN) printf("You won!\n");
	else if(game.winner==COMPUTER) printf("I won!!!!\n");
	else printf("Draw :(\n");
}

/*
* Get a players move
*/
int player_move(TicTacToe *gameptr){
	int x,y;
	int valid;
	printf("Enter coordinates (row, column) for your move: ");
	scanf("%d%*c%d", &x, &y);
	x--; y--;
	if(x >= gameptr->size || y >= gameptr->size || x < 0 || y < 0){
		printf("Invalid move, position outside the board. Try again.\n");
		valid = FALSE;
	} else if(gameptr->board[x][y] != NONE) {
		printf("Invalid move, place already taken. Try again.\n");
		valid =  FALSE;
	} else {
		gameptr->board[x][y] = HUMAN;
		valid = TRUE;
	}
	return valid;
}

/*
* Get a move from the computer. It just plays the
* next free space so it's not really an AI
*/
void computer_move(TicTacToe *gameptr){
	int done = FALSE;
	int i,j,cx,cy;
	cx = cy = -1;
	for(i = 0; i<gameptr->size; i++){
		for(j = 0; j<gameptr->size; j++)
			if(gameptr->board[i][j]==NONE){
				cx = i; cy = j;
				break;
			}
			if (cx != -1) {
				gameptr->board[cx][cy] = COMPUTER;
				break;
			}
	}
}

/*
* Map the board token ID to a character
*/
char tokenstr(int t){
	if(t==HUMAN)
		return 'X';
	else if (t==COMPUTER)
		return 'O';
	return '.';
}

/*
* See if there is a winner to the gameptr
* return true (0) if so otherwise false
*/
int check(TicTacToe *gameptr)
{
	int count;
	int x_sum;
	int x, y;
	int y_sum;

	/* Checks rows */
	for (y = 0; y < gameptr->size; y++) {
		x_sum = 0;
		for (x = 0; x < gameptr->size; x++)
			x_sum += gameptr->board[y][x];
		if (x_sum == gameptr->size || x_sum == (gameptr->size * -1)){
		if (x_sum < 0){
			gameptr->winner = HUMAN;
		} else {
			gameptr->winner = COMPUTER;
		}
		return TRUE;
		}
	}

	/* Checks vertical */
	for (y = 0; y < gameptr->size; y++) {
		y_sum = 0;
		for (x = 0; x < gameptr->size; x++)
			y_sum += gameptr->board[x][y];
		if (y_sum == gameptr->size || y_sum == (gameptr->size * -1)){
		if (y_sum < 0){
			gameptr->winner = HUMAN;
		} else {
			gameptr->winner = COMPUTER;
		}
			return TRUE;
		}
	}
	/*
	* Checks the bottom right to top left diagonal
	*
	* X . .
	* . X .
	* . . X
	*/
	int diagonal_sum = 0;
	int i, j;
	for (i = 0; i < gameptr->size; i++) {
		diagonal_sum += gameptr->board[i][i];
	}
	if (diagonal_sum == gameptr->size || diagonal_sum == (gameptr->size * -1)){
	if (diagonal_sum < 0){
		gameptr->winner = HUMAN;
	} else {
		gameptr->winner = COMPUTER;
	}
		return TRUE;
	}
	/*
	* Checks the bottom left to top right diagonal
	*
	* . . X
	* . X .
	* X . .
	*/
	diagonal_sum = 0;
	count = gameptr->size - 1;
	for (i = 0; i < gameptr->size; i++) {
		diagonal_sum += gameptr->board[i][count];
		count--;
	}
	if (diagonal_sum == gameptr->size || diagonal_sum == (gameptr->size * -1)){
	if (diagonal_sum < 0){
		gameptr->winner = HUMAN;
	} else {
		gameptr->winner = COMPUTER;
	}
	return TRUE;
	}
	
	/* Checks if no moves left */
	count = 0;
	for (i = 0; i < gameptr->size; i++) {
		for (j = 0; j < gameptr->size; j++) {
			if (gameptr->board[i][j] == NONE) count++;
		}
	}
	if (count == 0){
		gameptr->winner = DRAW;
		return TRUE;
	}
	/* No-one won and nor was there a draw */
	return FALSE;
}

/*
* Handles the memory freeing of the game
*/
void free_game(TicTacToe *gameptr){
	int i;
	for (i = 0; i < gameptr->size; i++)
		free(gameptr->board[i]);
	free(gameptr->board);
}
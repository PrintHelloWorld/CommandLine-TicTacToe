/*
 *
 * TICTACTOE -- human vs. computer
 *
 * Author: Ian Welch
 * Date: May 2015
 *
 * CHANGE LOG:
 *
 * 26.05 modified the user prompt to say (row,col) instead of x,y
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "tictactoe.h"

int check();
void init_game();
int player_move();
void computer_move();
void print_game();
char tokenstr(int token);
void print_result();

int board[3][3]; // our tic tac toe board
int winner; // who has won the game

int main(void)
{
  printf("This is the game of Tic Tac Toe.\n");
  printf("You will be playing against the computer.\n");

  init_game(); // initialise the board

  printf("The game board is %d by %d.\n",3,3);

  int done;
  do {
    print_game();
    do {
      done=player_move();
    } while (!done); // loop until valid move
    if(check() != FALSE) break; // was a winner or a draw
    computer_move();
    if(check()!=FALSE) break; // was a winner or a draw
  } while(TRUE);

  print_result();
  print_game(); /* show final positions */

  return 0;
}

/* Initialize the matrix. */
void init_game()
{
  int i, j;

  // now initialise it
  for(i=0; i<3; i++)
    for(j=0; j<3; j++)
      // set to empty of tokens 
      board[i][j] = NONE;
}

/* Get a player's move. */
int player_move()
{
  int x, y;
  int valid;

  // changed the print statement so it is clear that it is row,col (Ian 26.05)
  printf("Enter coordinates (row,column) for your move: ");
  scanf("%d%*c%d", &x, &y);

  x--; y--;

  if(board[x][y]!= NONE){
    printf("Invalid move, try again.\n");
    valid=FALSE;
  }
  else {
    board[x][y] = HUMAN;
    valid=TRUE;
  }
  return valid;
}

/* Get a move from the computer. */
void computer_move()
{
  int done = FALSE;
  int i,j,cx,cy;
  cx = cy = -1;
  for(i=0; i<3; i++){
    for(j=0; j<3; j++) 
      if(board[i][j]==NONE){
	cx = i; cy = j;
	break;
}
    if (cx != -1) {
      board[cx][cy] = COMPUTER;
      break;
    }
  }
}

/* Map the board token ID to a character. */
char tokenstr(int t) 
{
  if(t==HUMAN) 
    return 'X';
  else if (t==COMPUTER)
    return 'O';
  return '.';
}

/* Display the game board. */
void print_game()
{
  int i,j;

  // read and print the board one character at a time
  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      printf("%c",tokenstr(board[i][j]));
}
    printf("\n");
  }
  printf("\n");
}

/* See if there is a winner. */
/* return true (0) if so otherwise false */
int check() {
  int i,j;
  int count;

  for(i=0; i<3; i++) {  /* check rows */
    if(board[i][0] != NONE &&
       board[i][0]==board[i][1] &&
       board[i][0]==board[i][2]) {
      winner=board[i][0];
      return TRUE;
    }
  }

  for(i=0; i<3; i++) {  /* check columns */
    if(board[0][i] != NONE &&
       board[0][i]==board[1][i] &&
       board[0][i]==board[2][i]) {
      winner=board[0][i];
      return TRUE;
    }
  }

  /* test diagonals */
  if(board[0][0] != NONE &&
     board[0][0]==board[1][1] &&
     board[1][1]==board[2][2]) {
       winner=board[0][0];
       return TRUE;
  }

  if(board[0][2] != NONE &&
     board[0][2]==board[1][1] &&
     board[1][1]==board[2][0]) {
    winner=board[0][2];
    return TRUE;
  }
  
  /* test if out of space on the board */
  count=0;
  for(i=0; i<3; i++){
    for(j=0; j<3; j++) {
      if(board[i][j]==NONE) count++;
    }
  }
  if(count==0) {
    winner=DRAW;
    return TRUE;
  }

  // no-one and nor was there a draw
  return FALSE;
}

/* Print the result */
void print_result() 
{
  if(winner==HUMAN) printf("You won!\n");
  else if(winner==COMPUTER) printf("I won!!!!\n");
  else printf("Draw :(\n");
}

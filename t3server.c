#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tictactoe.h"

int check(TicTacToe* game);
void init_game(TicTacToe* game, int size);
void free_game(TicTacToe *game);
int player_move(int serverfd, int clientfd, TicTacToe* game);
void computer_move(TicTacToe* game);
void print_game(int clientfd, TicTacToe game);
char tokenstr(int token);
void print_result(int clientfd, TicTacToe game);
void client_continue(int clientfd);

int main(void)
{
  // define our variables related to pipes
  int clientfd, serverfd;
  char *clientpipe = "clientpipe";
  char *serverpipe = "serverpipe";

  // create the FIFO (named pipe) and open for reading 
  mkfifo(serverpipe, 0666);
  mkfifo(clientpipe, 0666);
  serverfd = open(serverpipe, O_RDONLY); // client talks to server
  clientfd = open(clientpipe, O_WRONLY); // server talks to client

  printf("This is the game of Tic Tac Toe.\n");
  printf("You will be playing against the computer.\n");

  int size; /* Size of board */

  read(serverfd, &size, sizeof(size));

  // wait for a client to tell us how large is the game board
  printf("The game board is %d by %d.\n",size, size);

  // initialise the board
  TicTacToe game;
  init_game(&game,size);

  // as for the generic solution with addition of pipes
  int done;
  char command;
  do{
    read(serverfd, &command, sizeof(command));
    if (command == 'p'){
      print_game(clientfd, game);
    } else if (command == 'm'){
      do {
        done = player_move(serverfd, clientfd, &game);
        write(clientfd, &done, sizeof(done));
      } while (!done);
      int over = check(&game);
      write(clientfd, &over, sizeof(over));
      if (over == 0){
        computer_move(&game);
        over = check(&game);
        write(clientfd, &over, sizeof(over));
      }
    } else if (command == '0') break;
  } while(TRUE);

  print_game(clientfd, game);

  print_result(clientfd, game);

  // clean up
  close(serverfd);
  unlink(serverpipe);
  free_game(&game);

  return 0;
}

/* Initialize the matrix. */
void init_game(TicTacToe* game,int size)
{
  int i, j;
  // set the size of the board
  int **board = (int **)malloc(sizeof(int *)*size);

  // use malloc to dynamically allocate 2D array
  for (i = 0; i < size; i++) {
    board[i] = (int *)malloc(sizeof(int)*size);
  }

  // now initialise it
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      board[i][j] = NONE;
    }
  }

  game->board = board;
  game->size = size;
  // set winner to undecided
  game->winner = FALSE;
}

/* Deallocate the dynamically allocated memory */
void free_game(TicTacToe* game)
{
  // code goes here, as for your generic solution
  int i;
  int size = game->size;
  for (i = 0; i < size; i++) {
    free(game->board[i]);
  }
  free(game->board);
}

/* Get a player's move. */
int player_move(int serverfd, int clientfd, TicTacToe* game)
{
  /* read move from client one int at a time */
  int x, y;

  read(serverfd, &x, sizeof(x));
  read(serverfd, &y, sizeof(y));

  /* check if valid move or not */
  int valid;

  if(x >= game->size || y >= game->size || x < 0 || y < 0){
    printf("Invalid move, position outside the board. Try again.\n");
    valid = 0;
  } else if(game->board[x][y] != NONE) {
    printf("Invalid move, space occupied. Try again.\n");
    valid=0;
  } else {
    game->board[x][y] = HUMAN;
    valid = 1;
  }

  /* return result to client */
  return valid;
}

/* Get a move from the computer. */
/* Return true (not 0) if can move */
/* Return false () if cannot move */
void computer_move(TicTacToe* game)
{
  // as for your generic solution
  int i, j;
  int cx,cy;
  cx = cy = -1;
  for(i = 0; i<game->size; i++){
    for(j = 0; j<game->size; j++)
      if(game->board[i][j]==NONE){
        cx = i; cy = j;
        break;
      }
    if (cx != -1) {
      game->board[cx][cy] = COMPUTER;
      break;
    }
  }
}

/* Map the board token ID into a character */
char tokenstr(int t)
{
  if(t==HUMAN)
    return 'X';
  else if (t==COMPUTER)
    return 'O';
  return '.';
}

/* Send the game result to the client */
/* Do it a character at a time -- easy! */
void print_game(int clientfd, TicTacToe game)
{
  int i,j;
  // write out the board one character at a time
  int size = game.size;
  for (i = 0; i < size; ++i) {
    for (j = 0; j < size; ++j) {
      int point = game.board[i][j];
      char c = tokenstr(point);
      printf("%c ", c); /* Display to server */
      write(clientfd, &point, sizeof(point));
    }
    printf("%c", '\n'); /* Display to server */
    int new_line = 10;
    write(clientfd, &new_line, sizeof(new_line));
  }
  printf("%c", '\n'); /* Display to server */
  client_continue(clientfd);
}

/* See if there is a winner. */
/* return true (0) if so otherwise false */
int check(TicTacToe* game)
{
  // code is the same as your generic version
  int count;
  int x_sum;
  int x, y;
  int y_sum;

  /* Checks horizontal */
  for (y = 0; y < game->size; y++) {
    x_sum = 0;
    for (x = 0; x < game->size; x++) {
      x_sum += game->board[y][x];
    }
    if (x_sum == game->size || x_sum == (game->size * -1)){
      if (x_sum < 0){
        game->winner = HUMAN;
      } else {
        game->winner = COMPUTER;
      }
      return TRUE;
    }
  }

  /* Checks vertical */
  for (y = 0; y < game->size; y++) {
    y_sum = 0;
    for (x = 0; x < game->size; x++) {
      y_sum += game->board[x][y];
    }
    if (y_sum == game->size || y_sum == (game->size * -1)){
      if (y_sum < 0){
        game->winner = HUMAN;
      } else {
        game->winner = COMPUTER;
      }
      return TRUE;
    }
  }

   /* Checks the bottom right to top left diagonal */
  int i, j;
  int diagonal_sum = 0;
  for (i = 0; i < game->size; i++) {
    diagonal_sum += game->board[i][i];
  }
  if (diagonal_sum == game->size || diagonal_sum == (game->size * -1)){
    if (diagonal_sum < 0){
      game->winner = HUMAN;
    } else {
      game->winner = COMPUTER;
    }
    return TRUE;
  }

  /* Checks the bottom left to top right diagonal */
  diagonal_sum = 0;
  count = game->size - 1;
  for (i = 0; i < game->size; i++) {
    diagonal_sum += game->board[i][count];
    count--;
  }
  if (diagonal_sum == game->size || diagonal_sum == (game->size * -1)){
    if (diagonal_sum < 0){
      game->winner = HUMAN;
    } else {
      game->winner = COMPUTER;
    }
    return TRUE;
  }

  /* Checks if no moves left */
  count = 0;
  for (i = 0; i < game->size; i++) {
    for (j = 0; j < game->size; j++) {
      if (game->board[i][j] == NONE) count++;
    }
  }
  if (count == 0){
    game->winner = DRAW;
    return TRUE;
  }

  /* No-one won and nor was there a draw ? */
  return FALSE;
}

/* Tell the client that game has ended and the result of the game */
void print_result(int clientfd, TicTacToe game)
{
  // send game over
  int game_over = 0;
  write(clientfd, &game_over, sizeof(game_over));
  // tell the client the winner
  int winner = game.winner;
  write(clientfd, &winner, sizeof(winner));

  if (winner == 1){
    printf("\nComputer won\n");
  } else if (winner == 0){
    printf("\nDraw\n");
  } else if (winner == -1){
    printf("\nPlayer won\n");
  }
}

/* Tell the client to continue */
void client_continue(int clientfd)
{
  // tell the client that it should continue playing
  int i = 11;
  write(clientfd, &i, sizeof(i));
}
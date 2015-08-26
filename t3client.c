#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "tictactoe.h"

int init_game(int serverfd);
int check(int clientfd);
int player_move(int clientfd, int serverfd);
void print_game(int clientfd, int size);
void print_winner(int clientfd);

// implement a very simple client that sends and receives
// all other logic, just send and receive strings
// extension is to add extra clients
int main(void)
{
  // define our variables related to pipes
  int clientfd, serverfd;
  char *clientpipe = "clientpipe";
  char *serverpipe = "serverpipe";

  // create the FIFO (named pipe) and open for reading 
  mkfifo(clientpipe, 0666);
  mkfifo(serverpipe, 0666);
  serverfd = open(serverpipe, O_WRONLY);
  clientfd = open(clientpipe, O_RDONLY);

  printf("This is the game of Tic Tac Toe.\n");
  printf("You will be playing against the computer.\n");

  int done;  // used to check validity of move

  // client must send commands and wait for responses
  // client exits loop if game is over
  // client should continue if receives continue message

  /* Ask for board size */
  int size = init_game(serverfd);

  do{
    char command = 'p';
    write(serverfd, &command, sizeof(command));
    print_game(clientfd, size);
    do {
      command = 'm';
      write(serverfd, &command, sizeof(command));
      done = player_move(clientfd, serverfd);
    }while(!done); /* loop until valid move */
    /* double check to see if either player or computer has won */
    if (check(clientfd) != 0 || check(clientfd) != 0){
      command = '0'; /* Send terminal command to break server loop */
      write(serverfd, &command, sizeof(command));
      break;
    }

  } while (1);

  char command = 'p';
  write(serverfd, &command, sizeof(command));
  print_game(clientfd, size);

  /* display the winner */
  print_winner(clientfd);
  //free memory
  close(clientfd);
  unlink(clientpipe);

  return 0;
}

/* Start the game by telling server size of the board */
int init_game(int serverfd)
{
  // ask the player
  int size;

  printf("Enter the size of the board: ");

  if(scanf("%d", &size) != 1 || size<= 0){
	printf("Size must be of an Integer greater than 0. \n");
	return -1; /* main failed with errors */
  }
  // server command
  write(serverfd, &size, sizeof(size));
  return size;
}

/* Send player's move to the server and check if was valid. */
int player_move(int clientfd, int serverfd)
{
  int x,y;
  int valid = 0;

  do{
  	/* ask player */
    printf("Enter coordinates (row, column) for your move: ");
    scanf("%d%*c%d", &x, &y);

    x--; y--;
    /* send the details */
    write(serverfd, &x, sizeof(x));
    write(serverfd, &y, sizeof(y));

    /* check the result */

    read(clientfd, &valid, sizeof(valid));
  }while (valid == 0);
  return valid;
}

/* Display the game on the screen. */
void print_game(int clientfd, int size)
{
  // read and print the board one character at a time
  int printing = 1;
  while (printing)
  {
    int point;
    read(clientfd, &point, sizeof(point));
    if (point == 11){
      printing = 0;
    } else {
      if (point == 10){
        printf("\n");
      } else if (point == 0){
        printf(". ");
      } else if (point == 1) {
        printf("O ");
      } else if (point == -1){
        printf("X ");
      }
    }
  }
}

/* Check whether game has ended or whether computer is waiting 
   for the next move */
int check(int clientfd)
{
  int check = 0;
  read(clientfd, &check, sizeof(check));

  // someone did win, so the game is over
  return check;
}

/* Receive the final results from the server */
void print_winner(int clientfd)
{
  int game_over;
  read(clientfd, &game_over, sizeof(game_over));
  if (game_over == 0){
    int winner;
    read(clientfd, &winner, sizeof(winner));
    if (winner == 1){
      printf("\nI won!\n");
    } else if (winner == 0){
      printf("\nIt was a draw!\n");
    } else if (winner == -1){
      printf("\nYou won!\n");
    }
  } else {
    printf("\nPipe Failiure.\n");
  }
}

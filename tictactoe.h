/* Useful defines */
const int TRUE = 1;
const int FALSE = 0;

/* Board */
const int HUMAN = -1;
const int COMPUTER = 1;
const int NONE = 0;
const int DRAW = -2;

/* IPC messages */
const int MSG_GAME_OVER = 1;
const int MSG_CONTINUE = 0;

/* Struct to store game state */
typedef struct TicTacToe{
	int size; /* size of game board */
	int **board; /* game board 2d array [size][size] */
	int winner; /* who won, draw=-2, human win=-1, none=0, computer win =1; */
} TicTacToe;

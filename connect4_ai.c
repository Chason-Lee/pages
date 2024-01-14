#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define boardSize 168
#define size 4

int isValid(int (*)[6][7], int);
void makeMove(int (*)[6][7], int, int);
int isTerminal(int (*)[6][7]);
int evaluateSegment(int, int, int, int);
int evaluatePosition(int (*)[6][7]);
struct evaluation minimax(int (*)[6][7], int, int, int, int);
void printBoard(int (*)[6][7]);

const int AI = 1;
const int OPPONENT = -1;
int board[6][7] =  {{0, 0, 0, 0, 0, 0, 0},
			    	{0, 0, 0, 0, 0, 0, 0},
			    	{0, 0, 0, 0, 0, 0, 0},
				    {0, 0, 0, 0, 0, 0, 0},
			    	{0, 0, 0, 0, 0, 0, 0},
			    	{0, 0, 0, 0, 0, 0, 0}};

struct evaluation{
	int eval;
	int bestMove;
};


void main(){
	/*struct evaluation score = minimax(board, AI, 5, -INFINITY, INFINITY);
	printf("%i\n", score.eval);
	printf("%i\n", score.bestMove);*/
	/*struct evaluation e = minimax(&board, AI, 1, -INFINITY, INFINITY);
	printf("%i\n", e.eval);
	printf("%i\n", e.bestMove);*/
	int whoFirst = 0;
	do {
		printf("Who Goes first?:\n[1] Human\n[2] AI\n>>");
		scanf("%i", &whoFirst);
	}
	while(whoFirst == 0);
	
	if (whoFirst == 2) {
		makeMove(&board, 4-1, AI);
	}
	
	int gameOver = 0;
	while(gameOver == 0){
		int move;
		do {	
			printf("Make a move:");
			scanf("%i", &move);
		}
		while(isValid(&board,move-1) == 0);
		makeMove(&board, move-1, OPPONENT);
		struct evaluation e = minimax(&board, AI, 7, -INFINITY, INFINITY);
		makeMove(&board, e.bestMove, AI);
		printf("Best move is %i with a score of %i\n",e.bestMove+1,e.eval);	
		printBoard(&board);	
		if (isTerminal(&board) == 1) {
			printf("Game Over!\n");
			gameOver = 1;
		}
	}
}

int isValid(int (*board)[6][7], int column){
	if (column > 6) {
		return 0;
	} else {
		for(int i=0;i<6;i++){
			if ((*board)[i][column] == 0) {
				return 1;
			}
		}
	}
	return 0;
}

void makeMove(int (*board)[6][7], int column, int player){
	for(int i=0;i<6;i++){
		if ((*board)[i][column] == 0) {
			(*board)[i][column] = player;
			break;
		}	
	}
}

int isTerminal(int (*board)[6][7]){
	for(int y=0; y<6; y++) {
		for(int x=0; x<4; x++) {
			if (abs((*board)[y][x]+(*board)[y][x+1]+(*board)[y][x+2]+(*board)[y][x+3]) == 4){
				//printf("horizontal");
				return 1;
			}
		}
	}
	
	for(int x=0; x<7; x++) {
		for(int y=0; y<3; y++) {
			if(abs((*board)[y][x]+(*board)[y+1][x]+(*board)[y+2][x]+(*board)[y+3][x]) == 4){
				//printf("vertical");
				return 1;
			}
		}
	}
	
	for(int x=0; x<4; x++) {
		for(int y=0; y<3; y++) {
			if(abs((*board)[y][x] + (*board)[y+1][x+1] + (*board)[y+2][x+2] + (*board)[y+3][x+3]) == 4 || abs((*board)[5-y][x] + (*board)[5-y-1][x+1] + (*board)[5-y-2][x+2] + (*board)[5-y-3][x+3]) == 4){
				//printf("%i\n", abs((*board)[y][x] + (*board)[y+1][x+1] + (*board)[y+2][x+2] + (*board)[y+2][x+2]));
				//printf("%i\n", abs((*board)[5-y][x] + (*board)[5-y-1][x+1] + (*board)[5-y-2][x+2] + (*board)[5-y-3][x+3]));
				
				return 1;
			}
		}
	}
	
	return 0;
}

int evaluateSegment(int s1, int s2, int s3, int s4){
	int count[] = {0,0,0};
	int segment[] = {s1,s2,s3,s4};
	
	for(int i = 0; i < 4; i++){
		switch(segment[i]){
			case -1:
				count[0]++;
				break;
			case 0:
				count[1]++;
				break;
			case 1:
				count[2]++;
				break;
		}
	}
	
	
	if(count[0] == 4){
		//printf("lose\n");
		return -INFINITY;
	}
	if(count[2] == 4){
		//printf("win %i\n", INFINITY);
		return INFINITY;
	}
	
	if(count[0] == 3 && count[1] == 1){
		//printf("Segment: 3\n");
		return -5;
	}
	if(count[2] == 3 && count[1] == 1){
		//printf("Segment: 3\n");
		return 5;
	}
	
	if(count[0] == 2 && count[1] == 2){
		//printf("Segment: 2\n");
		return -2;
	}
	if(count[2] == 2 && count[1] == 2){
		//printf("Segment: 2\n");
		return 2;
	}
	return 0;
}

int evaluatePosition(int (*board)[6][7]){
	int scoreAdd = 0;
	int score = 0;
	for(int y=0; y<6; y++) {
		for(int x=0; x<4; x++) {
			scoreAdd = evaluateSegment((*board)[y][x],(*board)[y][x+1],(*board)[y][x+2],(*board)[y][x+3]);
			if (scoreAdd == (int)INFINITY || scoreAdd == (int)-INFINITY){
				return scoreAdd;
			} else {
				score += scoreAdd;
			}
			//printf("score: %i\n", score);
		}
	}
	
	for(int x=0; x<7; x++) {
		for(int y=0; y<3; y++) {
			scoreAdd = evaluateSegment((*board)[y][x],(*board)[y+1][x],(*board)[y+2][x],(*board)[y+3][x]);
				//printf("score: %i\n", score);
			if (scoreAdd == (int)INFINITY || scoreAdd == (int)-INFINITY){
				return scoreAdd;
			} else {
				score += scoreAdd;
			}
		}
	}
	
	for(int x=0; x<4; x++) {
		for(int y=0; y<3; y++) {
			scoreAdd = evaluateSegment((*board)[y][x],(*board)[y+1][x+1],(*board)[y+2][x+2],(*board)[y+3][x+3]);
			if (scoreAdd == (int)INFINITY || scoreAdd == (int)-INFINITY){
				return scoreAdd;
			} else {
				score += scoreAdd;
			}	
						
			//printf("score: %i\n", score);
			
			scoreAdd = evaluateSegment((*board)[5-y][x],(*board)[5-y-1][x+1],(*board)[5-y-2][x+2],(*board)[5-y-3][x+3]);
			if (scoreAdd == (int)INFINITY || scoreAdd == (int)-INFINITY){
				return scoreAdd;
			} else {
				score += scoreAdd;
			}
		}
	}
	
	return score;
}

struct evaluation minimax(int (*board)[6][7], int player, int depth, int alpha, int beta){
	int value;
	int bestMove;
	
	if(depth == 0 || isTerminal(board) == 1) {
		struct evaluation e = {evaluatePosition(board), 0};
		return e;
	}
	
	if (player == 1) {
		value = -INFINITY;
		bestMove = 0;
		
		for(int i = 0; i < 7; i++) {
			//usleep(500000);
			if(isValid(board, i) == 1){
				if (depth == 7){
					printf("---Column %i---\n", i+1);
				}
				int boardCopy[6][7];
				memcpy(boardCopy, board, boardSize);
				makeMove(&boardCopy, i, player);
				struct evaluation e = minimax(&boardCopy, player*-1, depth-1, alpha, beta);
				if (depth==7){
				printf("%i\n", e.eval);
				printf("%i\n", e.bestMove);
				//printf("%i\n", value);
				//printf("%i\n", e.eval > value);
			}
				//alpha = fmaxf(value, alpha);
				
				//if(alpha >= beta){
				//	break;
				//}
				
				if(e.eval > value){
					//if (depth ==7){
					//	printf("Eval: %i beat %i\n", e.eval, value);
					//	printf("Best move: %i beat %i\n", i, bestMove);
					//	printf("------------\n");
					//}
					value = e.eval;
					bestMove = i;
				
				}	
			}
		}
		//printf("Best move: %i\n", bestMove);
	}
	
	if (player == -1) {
		value = INFINITY;
		bestMove = 0;
		
		for(int i = 0; i < 7; i++) {
			
			if(isValid(board, i) == 1){
				int boardCopy[6][7];
				memcpy(boardCopy, board, boardSize);
				makeMove(&boardCopy, i, player);
				struct evaluation e = minimax(&boardCopy, player*-1, depth-1, alpha, beta);
				//beta = fminf(value, alpha);
				
				//if(alpha >= beta){
				//	break;
				//}
				
				if(e.eval < value){
					value = e.eval;
					bestMove = i;
					//printf(Best move is ....)
				}
			}
		}
	}
				//printf("%i\n", value);
				//printf("%i\n", bestMove);	
	struct evaluation e = {value, bestMove};
	return e;
}

void printBoard(int (*board)[6][7]){
	printf("----Board----\n");
	for(int i=0;i<6;i++){
		for(int x=0;x<7;x++){
			if ((*board)[5-i][x] == 1){
				printf("\033[93m X");
			}
			if ((*board)[5-i][x] == -1){
				printf("\033[31m O");
			} 
			if ((*board)[5-i][x] == 0){
				printf("\033[97m  ");
			}
		}
		printf("\033[97m\n");
		//printf("%i %i %i %i %i %i %i\n", (*board)[i][0],(*board)[i][1],(*board)[i][2],(*board)[i][3],(*board)[i][4],(*board)[i][5],(*board)[i][6]);
	}
	printf("\033[97m -------------\n");
	printf(" 1 2 3 4 5 6 7\n");
	printf("\033[0m");
}

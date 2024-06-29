#include <stdio.h>
#include <stdbool.h>
#include "keyPressUtils.h"

int getNewSelectorValue(int keyValue, int sti) {
	if (keyValue == 119 || keyValue == 87) { // 'w' or 'W'
		sti = sti-3 >= 0 ? sti-3 : sti+6;
	}else if (keyValue == 97 || keyValue == 65) { // 'a' or 'A'
		sti = sti-1 >= 0 ? sti-1 : 8;
	}else if (keyValue == 115 || keyValue == 83) { // 's' or 'S'
		sti = sti+3 <= 8 ? sti+3 : sti-6;
	}else if (keyValue == 100 || keyValue == 68) { // 'd' or 'D'
		sti = sti+1 <= 8 ? sti+1 : 0;
	}else sti = -1;
	return sti;
}

// returns tile (char) that's present in all positions of a horizontal row otherwise ' '
char charMatchingSameThreeInHorzRow(char gameValues[9][14]) {
	int i=0;
	int count=0;

	while (true) {
		if (gameValues[i][7] != gameValues[i+count][7]) { // adjacent tiles in a horizontal row don't match
			i+=3; // switch to next horizontal row index
			count=0;
		}else if (count < 2) { // same three in a row hasn't been found
			count++;
		}else if (count == 2) { // same three in a row has been found
			return gameValues[i][7];
		};

		if (i == 9) { // last horizontal row has been checked
			break;
		}
	}

	return ' ';
}

// returns tile (char) that's present in all positions of a vertical row otherwise ' '
char charMatchingSameThreeInVertRow(char gameValues[9][14]) {
	int i=0;
	int count=0;

	while (true) {
		if (gameValues[i][7] != gameValues[i+count][7]) { // adjacent tiles in a vertical row don't match
			i+=1; // switch to next vertical row index
			count=0;
		}else if (count < 6) {  // same three in a row hasn't been found
			count+=3;
		}else if (count == 6) {  // same three in a row has been found
			return gameValues[i][7];
		};
		if (i == 4) { // last vertical row has been checked
			break;
		}
	}

	return ' ';
}

// returns tile (char) that's present in all positions of a diagonal row otherwise ' '
char charMatchingSameThreeInDiagRow(char gameValues[9][14]) {
	int i=0;
	int count=0;

	while (true) {
		if (gameValues[i][7] != gameValues[i+count][7]) { // adjacent tiles in a diagonal row don't match
			if (i != 2) { // the first diagonal is still being iterated
				i=2;
				count=0;
			}else break;
		}else if ((i == 2 && count == 4) || count == 8) { // same three in a row has been found
			return gameValues[i][7];
		}else if (i == 0) { // We are checking left(\) diagonal.
			count+=4;
		}else if (i == 2) { // We are checking right(/) diagonal
			count+=2;
		};
	}
	return ' ';

}

char getWinnerChar(char gameValues[9][14]) {
	char sameCharAllInRow = ' ';
	sameCharAllInRow = charMatchingSameThreeInDiagRow(gameValues);

	if (sameCharAllInRow == ' ') { // no diagonal winning row was found
		sameCharAllInRow = charMatchingSameThreeInHorzRow(gameValues);
	}

	if (sameCharAllInRow == ' ') { // no horizontal winnning row was found
		sameCharAllInRow = charMatchingSameThreeInVertRow(gameValues);
	}

	return sameCharAllInRow;
	
}

int updateSelectorPos(char tiles[9][14], int sti, int new_sti) {
	if (new_sti == -1) { // invalid key was pressed
		tiles[sti][1] = '[';
		tiles[sti][11] = ']';
		return sti;
	};

	// removes the selector from the former position
	tiles[sti][1] = ' ';
	tiles[sti][11] = ' ';
	sti = new_sti;

	// moves selector to a new position
	tiles[sti][1] = '[';
	tiles[sti][11] = ']';
	return sti;
}

void displayHelpText(int player_turn) {
	printf("\e[1;1H\e[2J"); // clears the screen
	printf("\e[1m\e[92m------------------ TIC TAC TOE GAME! ----------------\e[m\n");
	printf("PLAYER 1: \e[1m\e[31mX\e[m\n");
	printf("PLAYER 2: \e[1m\e[34mO\e[m\n\n");
	printf("Use the W A S D keys to move the selector around\n"
				 "Press the X or O key when it's your turn to place your mark on a spot.\n");
	printf("Press q to quit at any time\n\n");

	if (player_turn == 1) {
		printf("\e[31mPLAYER ONE's MOVE\e[m\n");
	}else if (player_turn == 2) printf("\e[34mPLAYER TWO's MOVE\e[m\n");

}

int main(void) {
	int playerTurn = 1;
	bool gameOver = false;
	int n;
	int sti = 0; // Selected Tile Index
	char winnerChar = ' ';
	char tiles[9][14] = {
		{" [\e[30m \e[m] "}, {"  \e[30m \e[m  "}, {"  \e[30m \e[m  "},
		{"  \e[30m \e[m  "}, {"  \e[30m \e[m  "}, {"  \e[30m \e[m  "},
		{"  \e[30m \e[m  "}, {"  \e[30m \e[m  "}, {"  \e[30m \e[m  "},
	};

	while (!gameOver) {
		winnerChar = getWinnerChar(tiles); // gets the tile that fills all the positions of a row if any
		if (winnerChar != ' ') { // a player has won the game
			playerTurn = -1;
		}
		displayHelpText(playerTurn);

		printf("%s|%s|%s\n", tiles[0], tiles[1], tiles[2]);
		printf("-----------------\n"); 
		printf("%s|%s|%s\n", tiles[3], tiles[4], tiles[5]);
		printf("-----------------\n");
		printf("%s|%s|%s\n\n", tiles[6], tiles[7], tiles[8]);
		
		if (winnerChar == 'X') {
			printf("\e[92mPLAYER ONE WON!\e[m\n");
		}else if (winnerChar == 'O') {
			printf("\e[92mPLAYER TWO WON!\e[m\n");
		};
		n = getch();

		if (n == 81 || n == 113) { // 'q' || 'Q' key was pressed
			gameOver = true;
		};

		if (winnerChar != ' ') {
			continue;
		}

		if (n == 88 || n == 120) { // 'X' || 'x' key was pressed
			if (playerTurn == 1 && tiles[sti][7] == ' ') {
				// escape sequences plus char representing 'X' tile are inserted
				tiles[sti][4] = '3';
				tiles[sti][5] = '1';
				tiles[sti][6] = 'm';
				tiles[sti][7] = 'X';
				playerTurn = 2;
			}
		}else if (n == 79 || n == 111) { // 'O' || 'o' key was pressed
			if (playerTurn == 2 && tiles[sti][7] == ' ') {
				// escape sequences plus char representing 'X' tile are inserted
				tiles[sti][4] = '3';
				tiles[sti][5] = '4';
				tiles[sti][6] = 'm';
				tiles[sti][7] = 'O';
				playerTurn = 1;
			}
		}else { // try to move tile pos selector around
			int new_sti = getNewSelectorValue(n, sti); 
			sti = updateSelectorPos(tiles, sti, new_sti);
		} 
	}
	printf("\e[1;1H\e[2J"); // clear screen before exiting game.
	return 0;
}
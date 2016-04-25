#include <tlc5947.h>
#include "board.h"
#include <stdio.h>
#include <errno.h>

int main(void) {
    struct tlc5947 tlc5947;
	struct board board;
	char c;
    if(tlc5947_init(&tlc5947, 8, Wir_BRG)) {
		perror("error while initializing struct tlc5947\n");
		return 1;
	}
	board_init(&board, 0, 0);
	tlc5947_write(&tlc5947);
	while(board.state == game) {
		c = getchar();
		switch(c) {
			case 'w': {
				board_move(&board, up);
				break;
			}
			case 's': {
				board_move(&board, down);
				break;
			}
			case 'a': {
				board_move(&board, left);
				break;
			}
			case 'd': {
				board_move(&board, right);
				break;
			}
		}
		board_set(&board, &tlc5947);
		tlc5947_write(&tlc5947);
	}
	printf(board.state == win ? "\nwin\n" : "\nlose\n");
	tlc5947_free(&tlc5947);

    return 0;
}

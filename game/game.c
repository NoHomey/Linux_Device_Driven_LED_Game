#include <tlc5947.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "board.h"

void print(struct board* board) {
	int8_t x, y;
	printf("\n");
	for(y = BOARD_MAX; y >= 0; --y) {
		for(x = BOARD_MIN; x <= BOARD_MAX; ++x) {
			printf("%d ", _board_get(board, x, y));
		}
		printf("\n");
	}
	printf("\n");
}

void clear(void) {
	printf("\33c");
}

int main(void) {
    /*uint16_t i;
    struct tlc5947 tlc5947;
    uint16_t pwm[] = {500, 1000, 2000};
    tlc5947_init(&tlc5947, 8, Wir_BRG);
	tlc5947_free(&tlc5947);*/

	struct board board;
	int i;
	board_init(&board, 0, 0);
	clear();
	print(&board);
	for(i = 0; i < 20; ++i) {
		sleep(4);
		board_move(&board, down);
		print(&board);
	}
    return 0;
}

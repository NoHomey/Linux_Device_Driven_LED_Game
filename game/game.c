#include <tlc5947.h>
#include "board.h"
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <comio/comi.h>

int main(void) {
    struct tlc5947 tlc5947;
	struct board board[4];
	uint8_t byte;
	int number;
	int fd = comi_open();
	if(fd < 0) {
		perror("error while opening serial\n");
		return 1;
	}
    if(tlc5947_init(&tlc5947, 8, Wir_BRG)) {
		perror("error while initializing struct tlc5947\n");
		return 1;
	}
	comi_flush(fd);
	board_init(board, 0, 0);
	board_init(board + 1, 0, 4);
	board_init(board + 2, 4, 0);
	board_init(board + 3, 4, 4);
	board_set(board, &tlc5947);
	board_set(board + 1, &tlc5947);
	board_set(board + 2, &tlc5947);
	board_set(board + 3, &tlc5947);
	tlc5947_write(&tlc5947);
	while(1) {
		number = comi_do(fd, &byte);
		if(number == -1) {
			perror("error while reading\n");
			return 1;
		} else if(number) {
			printf("%d\n", byte);
			switch(byte) {
				case 26: {
					board_move(board, up);
					board_set(board, &tlc5947);
					break;
				}
				case 19: {
					board_move(board, left);
					board_set(board, &tlc5947);
					break;
				}
				case 13: {
					board_move(board, down);
					board_set(board, &tlc5947);
					break;
				}
				case 6: {
					board_move(board, right);
					board_set(board, &tlc5947);
					break;
				}
				case 21: {
					board_move(board + 1, up);
					board_set(board + 1, &tlc5947);
					break;
				}
				case 20: {
					board_move(board + 1, left);
					board_set(board + 1, &tlc5947);
					break;
				}
				case 16: {
					board_move(board + 1, down);
					board_set(board + 1, &tlc5947);
					break;
				}
				case 12: {
					board_move(board + 1, right);
					board_set(board + 1, &tlc5947);
					break;
				}
				case 5: {
					board_move(board + 2, up);
					board_set(board + 2, &tlc5947);
					break;
				}
				case 17: {
					board_move(board + 2, left);
					board_set(board + 2, &tlc5947);
					break;
				}
				case 27: {
					board_move(board + 2, down);
					board_set(board + 2, &tlc5947);
					break;
				}
				case 22: {
					board_move(board + 2, right);
					board_set(board + 2, &tlc5947);
					break;
				}
				case 18: {
					board_move(board + 3, up);
					board_set(board + 3, &tlc5947);
					break;
				}
				case 23: {
					board_move(board + 3, left);
					board_set(board + 3, &tlc5947);
					break;
				}
				case 24: {
					board_move(board + 3, down);
					board_set(board + 3, &tlc5947);
					break;
				}
				case 25: {
					board_move(board + 3, right);
					board_set(board + 3, &tlc5947);
					break;
				}
			}
			tlc5947_write(&tlc5947);
		}
	}
	tlc5947_free(&tlc5947);
	close(fd);

    return 0;
}

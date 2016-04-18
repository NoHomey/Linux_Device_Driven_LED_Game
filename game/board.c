#include "board.h"
#include <stdlib.h>

void _board_set(struct board* board, uint8_t x, uint8_t y, uint16_t val) {
	board->board[BOARD_CONVERT(x, y)] = val;
}

uint16_t _board_get(struct board* board, uint8_t x, uint8_t y) {
	return board->board[BOARD_CONVERT(x, y)];
}

uint8_t _board_new(struct board* board) {
	uint8_t i;
	time_t t;
	uint8_t zeros[BOARD_SIZE];
	uint8_t count = 0;
	for(i = 0; i < BOARD_SIZE; ++i) {
		if(board->board[i] == 0) {
			zeros[count++] = i;
		}
	}
	i = count > 0;
	if(i) {
		srand((unsigned) time(&t));
		board->board[zeros[rand() % count]] = 2;
	}
	return i;
}

void board_init(struct board* board, uint8_t x, uint8_t y) {
	uint8_t i;
	for(i = 0; i < BOARD_SIZE; ++i) {
		board->board[i] = 0;
	}
	board->x = x;
	board->y = y;
	board->is_4096 = 0;
	_board_new(board);
	_board_new(board);
}

void _board_move_left(struct board* board) {}
void _board_move_right(struct board* board) {}
void _board_move_up(struct board* board) {}
void _board_move_down(struct board* board) {}

void board_move(struct board* board, enum direction direction) {
	if(!board->is_4096) {
		switch(direction) {
			case left: {
				_board_move_left(board);
				break;
			}
			case right: {
				_board_move_right(board);
				break;
			}
			case up: {
				_board_move_up(board);
				break;
			}
			case down: {
				_board_move_down(board);
				break;
			}
		}
		_board_new(board);
	}
}

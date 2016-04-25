#include "board.h"
#include <stdlib.h>

void _board_set(struct board* board, uint8_t x, uint8_t y, uint8_t val) {
	if(BOARD_VALID(x) && BOARD_VALID(y)) {
		board->board[BOARD_CONVERT(x, y)] = val;
	}
}

uint8_t _board_get(struct board* board, uint8_t x, uint8_t y) {
	if(BOARD_VALID(x) && BOARD_VALID(y)) {
		return board->board[BOARD_CONVERT(x, y)];
	}
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
		board->board[zeros[rand() % count]] = BOARD_INIT_VALUE;
	}

	return i;
}

void _board_move_single(struct board* board, int8_t x, int8_t y, int8_t direction_x, int8_t direction_y) {
	int8_t next_x, next_y, change, value, next_value;
	while(BOARD_VALID(x) && BOARD_VALID(y)) {
		next_x = x + direction_x;
		next_y = y + direction_y;
		if(!((BOARD_VALID(next_x)) || (BOARD_VALID(next_y)))) {
			break;
		}
		value = _board_get(board, x, y);
		next_value = _board_get(board, next_x, next_y);
		change = 0;
		if(value) {
			if(next_value == 0) {
				change = BOARD_INIT_VALUE;
			} else if(next_value == value) {
				change = 2 * BOARD_INIT_VALUE;
			}
			if(change) {
				_board_set(board, next_x, next_y, value + change - BOARD_INIT_VALUE);
				_board_set(board, x, y, 0);
				if((value * change) == BOARD_MAX_VALUE) {
					board->state = win;
				}
			}
		}
		x += direction_x;
		y += direction_y;
	}
}

void _board_move_left(struct board* board) {
	int8_t x, y;
	for(y = BOARD_MIN; y <= BOARD_MAX; ++y) {
		for(x = BOARD_MIN + 1; x <= BOARD_MAX; ++x) {
			_board_move_single(board, x, y, -1, 0);
		}
	}
}

void _board_move_right(struct board* board) {
	int8_t x, y;
	for(y = BOARD_MIN; y <= BOARD_MAX; ++y) {
		for(x = BOARD_MAX; x >= BOARD_MIN; --x) {
			_board_move_single(board, x, y, 1, 0);
		}
	}
}

void _board_move_up(struct board* board) {
	int8_t x, y;
	for(x = BOARD_MIN; x <= BOARD_MAX; ++x) {
		for(y = BOARD_MAX - 1; y >= BOARD_MIN; --y) {
			_board_move_single(board, x, y, 0, 1);
		}
	}
}

void _board_move_down(struct board* board) {
	int8_t x, y;
	for(x = BOARD_MIN; x <= BOARD_MAX; ++x) {
		for(y = BOARD_MIN + 1; y <= BOARD_MAX; ++y) {
			_board_move_single(board, x, y, 0, -1);
		}
	}
}

void board_init(struct board* board, uint8_t x, uint8_t y) {
	uint8_t i;
	for(i = 0; i < BOARD_SIZE; ++i) {
		board->board[i] = 0;
	}
	board->x = x;
	board->y = y;
	board->state = game;
	_board_new(board);
	_board_new(board);
}

void board_move(struct board* board, enum direction direction) {
	if(board->state == game) {
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
		if(!_board_new(board)) {
			board->state = lose;
		}
	}
}

void board_set(struct board* board, struct tlc5947* tlc5947) {
	uint8_t value;
	uint16_t rgb, pwm[3];
	int8_t x, y;
	for(x = BOARD_MIN; x <= BOARD_MAX; ++x) {
		for(y = BOARD_MIN; y <= BOARD_MAX; ++y) {
			value = _board_get(board, x, y);
			rgb = (x + board->x) * tlc5947->chips + (y + board->y);
			switch(value) {
				case 0: { //BLACK
					pwm[0] = 0;
					pwm[1] = 0;
					pwm[2] = 0;
					break;
				}
				case 1: { //RED
					pwm[0] = 4095;
					pwm[1] = 0;
					pwm[2] = 0;
					break;
				}
				case 2: { //GREEN
					pwm[0] = 0;
					pwm[1] = 4095;
					pwm[2] = 0;
					break;
				}
				case 3: { //BROWN
					pwm[0] = 2661;
					pwm[1] = 655;
					pwm[2] = 655;
					break;
				}
				case 4: { //CYAN
					pwm[0] = 0;
					pwm[1] = 4095;
					pwm[2] = 4095;
					break;
				}
				case 5: { //ORANGE
					pwm[0] = 4095;
					pwm[1] = 1023;
					pwm[2] = 0;
					break;
				}
				case 6: { //BLUE
					pwm[0] = 0;
					pwm[1] = 0;
					pwm[2] = 4095;
					break;
				}
				case 7: { //YELLOW
					pwm[0] = 4095;
					pwm[1] = 4095;
					pwm[2] = 0;
					break;
				}
				case 8: { //VIOLET
					pwm[0] = 2293;
					pwm[1] = 0;
					pwm[2] = 4095;
					break;
				}
				case 9: { //INDIGO
					pwm[0] = 1187;
					pwm[1] = 0;
					pwm[2] = 1024;
					break;
				}
				case 10: { //PINK
					pwm[0] = 4095;
					pwm[1] = 3072;
					pwm[2] = 3235;
					break;
				}
				case 11: { //MAGENTA
					pwm[0] = 4095;
					pwm[1] = 0;
					pwm[2] = 4095;
					break;
				}
				case 12: { //LIME
					pwm[0] = 1024;
					pwm[1] = 1024;
					pwm[2] = 0;
					break;
				}
				case 13: { //PURPLE
					pwm[0] = 1024;
					pwm[1] = 0;
					pwm[2] = 1024;
					break;
				}
				case 14: { //WHITE
					pwm[0] = 4095;
					pwm[1] = 4095;
					pwm[2] = 4095;
					break;
				}
			}
			tlc5947_setRGBLED(tlc5947, rgb, pwm);
		}
	}
}

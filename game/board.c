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
	uint8_t k, value;
	uint16_t rgb, pwm[3];
	int8_t x, y;
	for(x = BOARD_MIN; x <= BOARD_MAX; ++x) {
		for(y = BOARD_MIN; y <= BOARD_MAX; ++y) {
			rgb = (x + board->x) * tlc5947->chips + (y + board->y);
			value = _board_get(board, x, y);
			k = (value <= BOARD_COMBINATION) ? 0 : (value <= 2 * BOARD_COMBINATION) ? 1 : 3;
			switch(value - (k * BOARD_COMBINATION)) {
				case 0: {
					pwm[0] = 0;
					pwm[1] = 0;
					pwm[2] = 0;
					break;
				}
				case 1: {
					pwm[0] = 1000 * (k + 1);
					pwm[1] = 0;
					pwm[2] = 0;
					break;
				}
				case 2: {
					pwm[0] = 0;
					pwm[1] = 1000 * (k + 1);
					pwm[2] = 0;
					break;
				}
				case 3: {
					pwm[0] = 0;
					pwm[1] = 0;
					pwm[2] = 1000 * (k + 1);
					break;
				}
				case 4: {
					pwm[0] = 1000 * (k + 1);
					pwm[1] = 1000 * (k + 1);
					pwm[2] = 0;
					break;
				}
				case 5: {
					pwm[0] = 0;
					pwm[1] = 1000 * (k + 1);
					pwm[2] = 1000 * (k + 1);
					break;
				}
				case 6: {
					pwm[0] = 1000 * (k + 1);
					pwm[1] = 0;
					pwm[2] = 1000 * (k + 1);
					break;
				}
				case 7: {
					pwm[0] = 1000 * (k + 1);
					pwm[1] = 1000 * (k + 1);
					pwm[2] = 1000 * (k + 1);
					break;
				}
			}
			tlc5947_setRGBLED(tlc5947, rgb, pwm);
		}
	}
}

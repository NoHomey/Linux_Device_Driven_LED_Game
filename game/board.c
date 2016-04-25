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
				case 0: {
					pwm[0] = 0;
					pwm[1] = 0;
					pwm[2] = 0;
					break;
				}
				case 1: {
					pwm[0] = 40;
					pwm[1] = 1884;
					pwm[2] = 4055;
					break;
				}
				case 2: {
					pwm[0] = 1228;
					pwm[1] = 1802;
					pwm[2] = 1597;
					break;
				}
				case 3: {
					pwm[0] = 1802;
					pwm[1] = 2375;
					pwm[2] = 3522;
					break;
				}
				case 4: {
					pwm[0] = 1925;
					pwm[1] = 2416;
					pwm[2] = 1024;
					break;
				}
				case 5: {
					pwm[0] = 1884;
					pwm[1] = 1638;
					pwm[2] = 3809;
					break;
				}
				case 6: {
					pwm[0] = 2047;
					pwm[1] = 4095;
					pwm[2] = 3399;
					break;
				}
				case 7: {
					pwm[0] = 2088;
					pwm[1] = 3932;
					pwm[2] = 696;
					break;
				}
				case 8: {
					pwm[0] = 2252;
					pwm[1] = 1105;
					pwm[2] = 286;
					break;
				}
				case 9: {
					pwm[0] = 2252;
					pwm[1] = 3809;
					pwm[2] = 3809;
					break;
				}
				case 10: {
					pwm[0] = 2239;
					pwm[1] = 2239;
					pwm[2] = 3031;
					break;
				}
				case 11: {
					pwm[0] = 2416;
					pwm[1] = 3276;
					pwm[2] = 3276;
					break;
				}
				case 12: {
					pwm[0] = 2457;
					pwm[1] = 0;
					pwm[2] = 2457;
					break;
				}
				case 13: {
					pwm[0] = 2539;
					pwm[1] = 2539;
					pwm[2] = 1515;
					break;
				}
				case 14: {
					pwm[0] = 3440;
					pwm[1] = 1802;
					pwm[2] = 1556;
					break;
				}
				case 15: {
					pwm[0] = 3983;
					pwm[1] = 3522;
					pwm[2] = 204;
					break;
				}
				case 16: {
					pwm[0] = 4095;
					pwm[1] = 0;
					pwm[2] = 2744;
					break;
				}
				case 17: {
					pwm[0] = 4095;
					pwm[1] = 1024;
					pwm[2] = 1024;
					break;
				}
				case 18: {
					pwm[0] = 4095;
					pwm[1] = 2826;
					pwm[2] = 245;
					break;
				}
				case 19: {
					pwm[0] = 4095;
					pwm[1] = 2848;
					pwm[2] = 3276;
					break;
				}
				case 20: {
					pwm[0] = 4095;
					pwm[1] = 3727;
					pwm[2] = 2990;
					break;
				}
				case 21: {
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

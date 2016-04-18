#ifndef _BOARD_H
#define _BOARD_H

#include <stdint.h>
#include <tlc5947.h>

#define BOARD_SIZE 16
#define BOARD_ROW 4
#define BOARD_CONVERT(x, y) BOARD_ROW * x + y

struct board {
		uint8_t x;
		uint8_t y;
		uint16_t board[BOARD_SIZE];
		uint8_t is_4096;
};

enum direction {
	left,
	right,
	up,
	down
};

void _board_set(struct board* board, uint8_t x, uint8_t y, uint16_t val);
uint16_t _board_get(struct board* board, uint8_t x, uint8_t y);
uint8_t _board_new(struct board* board);
void _board_move_left(struct board* board);
void _board_move_right(struct board* board);
void _board_move_up(struct board* board);
void _board_move_down(struct board* board);
uint16_t _board_convert(struct board*, uint8_t x, uint8_t y);

void board_init(struct board* board, uint8_t x, uint8_t y);
void board_move(struct board* board, enum direction direction);
void board_write(struct board* board, struct tlc5947* tlc5947);

#endif

#ifndef _BOARD_H
#define _BOARD_H

#include <stdint.h>
#include <tlc5947.h>

#define BOARD_ROW 4
#define BOARD_MIN 0
#define BOARD_MAX BOARD_ROW - 1
#define BOARD_INIT_VALUE 1
#define BOARD_MAX_VALUE 21
#define BOARD_SIZE BOARD_ROW * BOARD_ROW
#define BOARD_VALID(d) ((d >= BOARD_MIN) && (d <= BOARD_MAX))
#define BOARD_CONVERT(x, y) BOARD_ROW * x + y

enum state {
	game,
	win,
	lose
};

struct board {
	uint8_t x;
	uint8_t y;
	uint8_t board[BOARD_SIZE];
	enum state state;
};

enum direction {
	left,
	right,
	up,
	down
};

void _board_set(struct board* board, uint8_t x, uint8_t y, uint8_t val);
uint8_t _board_get(struct board* board, uint8_t x, uint8_t y);
uint8_t _board_new(struct board* board);
void _board_move_single(struct board* board, int8_t x, int8_t y, int8_t direction_x, int8_t direction_y);
void _board_move_left(struct board* board);
void _board_move_right(struct board* board);
void _board_move_up(struct board* board);
void _board_move_down(struct board* board);

void board_init(struct board* board, uint8_t x, uint8_t y);
void board_move(struct board* board, enum direction direction);
void board_set(struct board* board, struct tlc5947* tlc5947);

#endif

#include <tlc5947.h>
#include "board.h"

int main(void) {
    struct tlc5947 tlc5947;
	struct board board;
    //tlc5947_init(&tlc5947, 8, Wir_BRG);
	board_init(&board, 0, 0);
	//tlc5947_write(tlc5947);
	board_move(&board, down);
	//board_set(board, tlc5947);
	//tlc5947_write(tlc5947);
	//tlc5947_free(&tlc5947);

    return 0;
}

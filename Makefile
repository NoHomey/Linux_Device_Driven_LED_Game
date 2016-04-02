all: clean
	gcc ./game.c -Wall -Werror -ltlc5947 -o game

clean:
	rm -f ./game

all: clean board.o ltlc5947
	#cd ../kernel_modules/tlc5947 && make remove all load device
	gcc board.o game.c -Wall -Werror -ltlc5947 -o game
	./game

ltlc5947:
	cd ../lib/tlc5947 && make

board.o:
	gcc -c board.c

clean:
	rm -f ./game *.o
	cd ../lib/tlc5947 && make clean

unload:
	cd ../kernel_modules/tlc5947 && make remove unload clean

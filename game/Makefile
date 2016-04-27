game: clean libtlc5947 libcomio board.o
	gcc board.o game.c -Wall -Werror -ltlc5947 -lcomi -o game
	./game

input: clean libcomio
	gcc input.c -Wall -Werror -lcomo -o input
	./input

libtlc5947:
	cd ../lib/tlc5947 && make

board.o:
	gcc -c board.c

libcomio:
	cd ../lib/comio && make

clean:
	rm -f ./game *.o ./input
	cd ../lib/tlc5947 && make clean
	cd ../lib/comio && make clean

unload:
	cd ../kernel_modules/tlc5947 && make remove unload clean

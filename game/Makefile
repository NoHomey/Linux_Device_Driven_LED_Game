all: clean board.o
	cd ../lib/tlc5947 && make
	#cd ../kernel_modules/tlc5947 && make remove all load device
	gcc game.c -Wall -Werror -ltlc5947 -o game board.o
	./game

board.o:
	gcc -c board.c

clean:
	rm -f ./game *.o
	cd ../lib/tlc5947 && make clean

unload:
	cd ../kernel_modules/tlc5947 && make remove unload clean

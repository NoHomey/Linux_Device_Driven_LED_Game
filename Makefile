all:
	cd ./tlc5947 && make
	cd ./kernel_modules/tlc5947 && make unload remove all load device
	gcc ./game.c -Wall -Werror -ltlc5947 -o game
	./game

clean:
	rm -f ./game
	cd ./tlc5947 && make clean
	cd ./kernel_modules/tlc5947 && make unload remove clean


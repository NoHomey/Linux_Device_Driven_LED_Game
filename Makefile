all:
	cd ./tlc5947 && make
	cd ./kernel_modules/tlc5947 && make remove all load device
	gcc ./game.c -Wall -Werror -ltlc5947 -o game
	./game

clean: unload
	rm -f ./game
	cd ./tlc5947 && make clean

unload:
	cd ./kernel_modules/tlc5947 && make unload remove clean

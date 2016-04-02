all: build game

build:
	cd ./tlc5947 && make
	cd ./kernel_modules/tlc5947 && make remove all load device

game: remove
	gcc ./game.c -Wall -Werror -ltlc5947 -o game
	./game

remove:
	rm -f ./game

clean: remove unload
	cd ./tlc5947 && make clean

unload:
	cd ./kernel_modules/tlc5947 && make unload remove clean

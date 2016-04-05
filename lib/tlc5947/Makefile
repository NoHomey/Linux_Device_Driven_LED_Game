all: clean
	gcc -c -Wall -Werror -fpic tlc5947.c
	gcc -shared -o libtlc5947.so tlc5947.o
	mv ./libtlc5947.so /usr/lib
	cp ./tlc5947.h /usr/include
	chmod 755 /usr/lib/libtlc5947.so
	ldconfig

clean:
	rm -f ./tlc5947.o /usr/lib/libtlc5947.so
	ldconfig

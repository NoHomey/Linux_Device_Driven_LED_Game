all: clean
	gcc -c -Wall -Werror -fpic comi.c
	gcc -shared -o libcomi.so comi.o
	gcc -c -Wall -Werror -fpic como.c
	gcc -shared -o libcomo.so como.o
	mv libcomi.so /usr/lib
	mv libcomo.so /usr/lib
	chmod 755 /usr/lib/libcom*.so
	rm *.o
	mkdir /usr/include/comio
	cp comio.h /usr/include/comio
	cp comi.h /usr/include/comio
	cp como.h /usr/include/comio
	chmod -R 555 /usr/include/comio
	ldconfig

clean:
	rm -f ./*/*.o
	rm -fR /usr/lib/libcom*.so /usr/include/comio
	ldconfig

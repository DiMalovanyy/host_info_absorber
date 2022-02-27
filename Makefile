absorber: main.o
	gcc -o absorber main.o

main.o: src/main.c
	gcc -c -o main.o -L/usr/local/lib/ -I/usr/local/include -Isrc/utils src/main.c

install_deps:
	git submodule init

clean: 
	rm -f *.o
	rm -f absorber




absorber: main.o
	gcc -o absorber main.o

main.o: src/main.c
	gcc -c -o main.o src/main.c

clean: 
	rm -f *.o
	rm -f absorber




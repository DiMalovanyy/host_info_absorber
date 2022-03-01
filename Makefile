
CUR_DIR=$(shell pwd)

INCLUDE_DIRS=-I/usr/local/include\
			 -I$(CUR_DIR)/src/

LIBRARY_DIRS=-L/usr/local/lib
LIBRARY=-ldns

all: absorber absorber_test
	@echo "[Make]: all"

absorber: main.o csv.o
	@echo "[Make]: executable target"
	gcc -o absorber $(INCLUDE_DIRS) $(LIBRARY_DIRS) $(LIBRARY) main.o csv.o

main.o: src/main.c
	gcc -c -o main.o src/main.c

csv.o: src/utils/csv/csv.c
	@echo "[Make]: compile csv framework"
	gcc -c -o csv.o src/utils/csv/csv.c -g

absorber_test: test.o test_csv.o csv.o
	@echo "[Make]: test target"
	gcc -o absorber_test $(INCLUDE_DIRS) $(LIBRARY_DIRS) test.o test_csv.o csv.o -g

test.o: test/test.c
	gcc -c -o test.o test/test.c -g

test_csv.o: test/csv/test_csv.c
	gcc -c -Itest/ -Isrc/ -o test_csv.o test/csv/test_csv.c -g


install_deps:
	git submodule init

clean: 
	rm -f *.o
	rm -f absorber
	rm -f absorber_test




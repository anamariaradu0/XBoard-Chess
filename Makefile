CFLAGS = -Wall
DEPS = Cell.h Board_utils.h

build:
	g++ Main.cpp -o a.out
run:
	./a.out
clean:
	rm -f a.out
	rm -rf dummy.txt

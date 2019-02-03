build:
	mkdir -p build && gcc -o build/tetris src/* -lncurses
run:
	./build/tetris

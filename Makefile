build:
	mkdir -p build && gcc -o build/tetris src/* -lncurses
clean:
	rm -r build
rebuild: clean build
run: build
	./build/tetris

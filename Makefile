build:
	mkdir -p build && gcc -o build/tetris src/* -lncurses || rm -d build
clean:
	rm -r build
rebuild: clean build
run: build
	./build/tetris

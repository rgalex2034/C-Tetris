# C-Tetris
A Tetris copy made on C, with the only purpose of learning C.

## Description
The game tries to mimic Tetris behaviour using **ncurses** to manipulate terminal output.
This game relies on some functions from `unistd.h`, so I don't expect it to compile on windows.

## Dependencies
The only third party dependency to build against is ncurses.
You can install it on Debian based systems with:
`sudo apt-get install libncurses5-dev`.

As it uses **gcc**, you should install `build-essential` package.

Install it with: `sudo apt-get install build-essential`

## Compiling/Installation
I used **make** to provide an easy compile/run command, but if you take a look at the Makefile, you'll see that it is the most basic
thing you can find.

 - To compile, execute on the root directory of this project: `make build`
 - To run, execute on the root directory of this project (After compiling): `make run`
 
It basically uses `gcc` with `-lncurses` library option, and outputs the binary on `build/tetris`.

### Alternatively
If you want to make the same thing without **make**, the equivalent of those two commands are:
 - Compile: `mkdir -p build && gcc -o build/tetris src/* -lncurses`
 - Run: `./build/tetris`

### Installation
There is no **installation** process done yet, but as it's a simple binary
you could copy it on a directory that is set on your `$PATH` or add a new directory on `$PATH`
with the `tetris` binary from the build folder inside.

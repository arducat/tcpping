MAKE ?= make
CC := clang++
LDFLAGS := "-lQt6Network -lQt6Core -lQt6Gui"
.PHONY: all clean prepare build link

all: prepare build link

prepare: clean
	mkdir -p tmp
	mkdir -p build

build:
	$(CC) -c src/main.cpp -I/usr/include/qt6 -o tmp/main.o

link:
	$(CC) tmp/main.o -L/usr/lib/qt6/qml/QtNetwork -lQt6Network -lQt6Core -lQt6Gui -o build/main 
	
clean:
	rm -rf tmp build

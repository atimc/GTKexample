

mygtkdemo : mygtkdemo.o
	gcc -Wall -o mygtkdemo mygtkdemo.o `pkg-config --libs gtk+-3.0`

mygtkdemo.o : mygtkdemo.c makefile
	gcc -Wall -g -c mygtkdemo.c `pkg-config --cflags gtk+-3.0`

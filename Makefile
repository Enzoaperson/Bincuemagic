CC=gcc
LIBS=-lmd -lb64
OUT=bincuemagic

main: ./src/main.c
	mkdir -p ./build
	$(CC) ./src/*.c -o ./build/$(OUT) $(LIBS)
	echo "Bincuemagic should now be compiled! You can run it using ./bincuemagic while in the build folder!"

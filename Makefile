CC=clang
CFLAGS=-Wall -g
TARGET=main

all: $(TARGET)

$(TARGET): main.o byteContainer.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o byteContainer.o

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

byteContainer.o: byteContainer.c byteContainer.h 
	$(CC) $(CFLAGS) -c byteContainer.c

clean:
	rm -f *.o $(TARGET)


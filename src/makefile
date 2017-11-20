CC=gcc
CFLAGS=-Wall
TARGET=sp_program
OBJECTS=main.o smallsh.o lim_youngguk.o bae_minsu.o han_sangwoo.o common.o prompt.o

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm *.o $(TARGET)

CC=gcc
CFLAGS=-Wall
TARGET=sp_program
OBJECTS=main.o smallsh.o df.o du.o ps.o print_recode.o

all: $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm *.o $(TARGET)

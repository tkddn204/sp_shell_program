CC = gcc
CFLAGS = -W -Wall
TARGET = sp_program
SRCDIR = src/
OBJDIR = obj/
OBJECTS  := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all : $(TARGET)

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
    $(CC) $(CFLAGS) -c $< -o $@
    @echo "Compiled "$<" successfully!"

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

clean :
	rm *.o $(TRAGET)

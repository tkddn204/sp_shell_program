CC=gcc
CFLAGS=-Wall
TARGET=sp_program
OBJECTS=main.o smallsh.o command_parser.o prompt.o
PROJECT_OBJECTS=project_alias.o project_cd.o project_df.o project_du.o project_history.o project_pipe_redirection.o project_ps.o

all: $(TARGET)

$(TARGET) : $(OBJECTS) $(PROJECT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm *.o $(TARGET)

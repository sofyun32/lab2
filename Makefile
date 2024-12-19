CC = gcc
CFLAGS = -Wall -g
LDFLAGS = 

SRC = main.c
OBJ = $(SRC:.c=.o)
TARGET = sed_simplified

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(TARGET) $(OBJ)

test: 
	./$(TARGET) input.txt -r "old" "new"
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
LDFLAGS = -lm
SRC = main.c mongoose/mongoose.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

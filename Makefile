CC = gcc
CFLAGS = -Wall -Wextra
SRC = main.c mongoose/mongoose.c
TARGET = server

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

CC = gcc
CFLAGS = -D_POSIX_C_SOURCE=200112L -Wno-int-to-pointer-cast
LDFLAGS = -lrt
SOURCES = mongoose.c server.c  # Замените server.c на ваш главный файл
TARGET = server

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

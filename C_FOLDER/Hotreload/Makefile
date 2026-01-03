CC      := gcc
CFLAGS  := -O2 -Wall -Wextra -pedantic -std=c2x -g
LDFLAGS := -ldl

TARGET  := Hotreload
SHARED  := shared.so

.PHONY: all clean run

all: $(TARGET) $(SHARED)

run: $(TARGET)
	./$(TARGET) ./$(SHARED)

$(TARGET): Hotreload.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

Hotreload.o: Hotreload.c Hotreload.h
	$(CC) $(CFLAGS) -c $< -o $@

$(SHARED): shared.c
	$(CC) -fPIC -shared $(CFLAGS) $< -o $@

clean:
	rm -f *.o $(TARGET) $(SHARED)


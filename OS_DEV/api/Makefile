flags=-O2 -std=c2x -I.
ldflags=

.PHONY: all clean

all: clean api.o

api.o: api.c api.h os.h
	gcc $(flags) $(CFLAGS) -c $<

clean:
	rm -rf *.o


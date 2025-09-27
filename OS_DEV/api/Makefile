flags=-O2 -std=c2x -I.
ldflags=-ldl -shared -fPIC

.PHONY: all clean

all: clean api.so

api.so: api.o disk.o stdoslib.o fs.o
	gcc $(flags) $(CFLAGS) $(ldflags)  $^ -o $@

api.o: api.c api.h os.h
	gcc $(flags) $(CFLAGS) -fPIC -c $<

disk.o: disk.c disk.h
	gcc $(flags) $(CFLAGS) -fPIC -c $<

stdoslib.o: stdoslib.c stdoslib.h
	gcc $(flags) $(CFLAGS) -fPIC -c $<
fs.o: fs.c fs.h
	gcc $(flags) $(CFLAGS) -fPIC -c $<

clean:
	rm -rf *.o *.so


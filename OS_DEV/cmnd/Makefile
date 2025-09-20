flags=-O2 -std=c23 -I../api
ldflags=

.PHONY: all clean

all:clean cmnd 

../api/api.o: ../api/api.c
	$(MAKE) -C ../api -sk "CFLAGS=$(flags)"

cmnd: cmnd.o ../api/api.o
	gcc $(flags) $^ -o $@ $(ldflags)

cmnd.o: cmnd.c cmnd.h
	gcc $(flags) -c $<

clean:
	rm -rf *.o cmnd


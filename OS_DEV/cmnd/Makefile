flags=-O2 -std=c23 -I../api
ldflags=

.PHONY: all clean clear

all:clean clear cmnd 

../api/api.o: ../api/api.c
	$(MAKE) -C ../api -sk "CFLAGS=$(flags)" clean all

cmnd: cmnd.o ../api/api.o
	gcc $(flags) $^ -o $@ $(ldflags)

cmnd.o: cmnd.c cmnd.h
	gcc $(flags) -c $<
clear:
	cd ../api
	$(MAKE) clean
clean:
	rm -rf *.o cmnd


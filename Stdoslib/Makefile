NAME := stdoslib
PREFIX ?= /usr/local
INCLUDEDIR := $(PREFIX)/include
LIBDIR := $(PREFIX)/lib
OBJ := stdoslib.o
STATIC := lib$(NAME).a
SHARED := lib$(NAME).so
CC := gcc
CFLAGS := -O2 -fPIC -Iinclude 
AR := ar
ARFLAGS := rcs
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

all: $(STATIC) $(SHARED)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -c $@

$(STATIC): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(SHARED): $(OBJ)
	$(CC) -shared -o $@ $^

install: all
	mkdir -p $(DEST)$(INCLUDEDIR)
	mkdir -p $(DEST)$(LIBDIR)
	cp include/$(NAME).h $(DEST)$(INCLUDEDIR)/
	cp $(STATIC) $(DEST)$(LIBDIR)/
	cp $(SHARED) $(DEST)$(INCLUDEDIR)/
	ldconfig || true

uninstall: 
	rm -f $(DEST)$(INCLUDEDIR)/$(NAME).h
	rm -f $(DEST)$(LIBDIR)/$(SHARED)
	rm -f $(DEST)$(LIBDIR)/$(STATIC)
	ldconfig || true


.PHONY: all install uninstall clean

clean:
	rm -rf $(OBJ) $(STATIC) $(SHARED)

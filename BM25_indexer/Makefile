COMPILER := g++
flags:= -Wall -Wextra -Wpedantic -std=c++20 -g -O0 -I/includes
objs:= main.o Stemmer.o stopwords.o utility.o bm25.o fzf_tui.o
APP := BM25_indexer
INSTALL_PATH := /usr/bin/$(APP)
all : clean main sudo_cl

.PHONY:all clean sudo_cl

BM25_indexer: $(objs)
	$(COMPILER) $(flags) $^ -o $@

main.o:includes/main.h includes/errors.h includes/Stemmer.h includes/stopwords.h includes/utility.h includes/bm25.h
	$(COMPILER) $(flags) -c main.cpp

Stemmer.o:includes/Stemmer.h includes/utility.h
	$(COMPILER) $(flags) -c Stemmer.cpp

stopwords.o:includes/stopwords.h
	$(COMPILER) $(flags) -c stopwords.cpp

utility.o:includes/utility.h
	$(COMPILER) $(flags) -c utility.cpp

bm25.o : includes/bm25.h
	$(COMPILER) $(flags) -c bm25.cpp

fzf_tui.o: includes/fzf_tui.h
	$(COMPILER) $(flags) -c fzf_tui.cpp

sudo_cl:
	rm -rf *.o

clean:
	rm -rf *.o main BM25_indexer

install: BM25_indexer
	 @if [ "$$EUID" -ne 0 ]; then \
                echo "Run as ROOT: sudo make install"; \
                exit 1; \
    fi; \
    REAL_USER=$$SUDO_USER; \
    USER_HOME=$$($(USER_HOME_CMD)); \
    echo "Installing for user: $$REAL_USER"; \
    echo "Home: $$USER_HOME"; \
    \
    echo "Installing executable to $(INSTALL_PATH)"; \
    cp $(APP) $(INSTALL_PATH); \
    chmod +x $(INSTALL_PATH); \
    \
	echo "Install complete. Run: $(APP)"

uninstall:
        @if [ "$$EUID" -ne 0 ]; then \
                echo "Run with sudo: sudo make uninstall"; \
                exit 1; \
        fi; \
        REAL_USER=$$SUDO_USER; \
        USER_HOME=$$($(USER_HOME_CMD)); \
        echo "Removing executable"; \
        rm -f $(INSTALL_PATH); \
        echo "Uninstalled."

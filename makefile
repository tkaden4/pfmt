SOURCES=$(wildcard ./src/*.c)
LIB_SOURCES=$(filter-out ./src/main.c, $(SOURCES))
LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -pipe -O2 -Wno-unused-variable \
	-Wno-unused-function -std=gnu99 -I./include/
LDFLAGS=
NAME=pfmt
EXE=$(NAME)
LIB=$(NAME).so
DISCARD=$(OBJECTS) $(EXE) lib/$(LIB)

# check and see if we have ccache
HAS_CCACHE := $(shell command -v ccache 2> /dev/null)
ifdef HAS_CCACHE
	CC := ccache $(CC)
else
	CC := $(CC)
endif

.PHONY: clean tst run install profile

all: $(EXE) $(LIB)

$(EXE) : $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(LIB_OBJECTS): $(LIB_SOURCES)
	$(CC) -o $@ -fpic -c $^ $(CFLAGS) $(LDFLAGS)

$(LIB) : $(LIB_OBJECTS)
	$(CC) -shared -o lib/$@ $^

install: install-exe install-lib

install-exe:
	install -s $(EXE) /usr/local/bin/$(NAME)
	# TODO no manfile yet
	# cp $(NAME).1 /usr/local/man/man1/
	# mandb
	
install-lib:
	cp lib/$(LIB) /usr/local/lib/$(LIB)

run:
	./pfmt -x 10 -y 10 -b ff0000 -f 00ff00 "Hello!"

clean:
	$(RM) $(DISCARD)

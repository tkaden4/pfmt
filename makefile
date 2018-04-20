SOURCES=$(wildcard ./src/*.c)
OBJECTS=$(SOURCES:.c=.o)
CCFLAGS=-Wall -Wextra -Werror -pipe -O2 -Wno-unused-variable \
	-Wno-unused-function -std=gnu99 -I./include/
LDFLAGS=
NAME=pfmt
EXE=$(NAME)
DISCARD=$(OBJECTS) $(EXE)

# check and see if we have ccache
HAS_CCACHE := $(shell command -v ccache 2> /dev/null)
ifdef HAS_CCACHE
	CC := ccache $(CC)
else
	CC := $(CC)
endif

.PHONY: clean tst run install profile

all: $(EXE)

$(EXE) : $(OBJECTS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

install:
	install -s $(EXE) /usr/local/bin/$(NAME)
	# cp $(NAME).1 /usr/local/man/man1/
	mandb

run:
	./pprint

clean:
	$(RM) $(DISCARD)

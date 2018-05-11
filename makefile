SOURCES=$(wildcard ./src/*.c)
LIB_SOURCES=$(filter-out ./src/main.c, $(SOURCES))
LIB_OBJECTS=$(LIB_SOURCES:.c=.o)
OBJECTS=$(SOURCES:.c=.o)
CFLAGS=-Wall -Wextra -Werror -pipe -O2 -Wno-unused-variable \
	-Wno-unused-function -std=gnu99 -I./include/
LDFLAGS=
NAME=pfmt
EXE=${NAME}
LIB_NAME=lib${NAME}
OUT=./lib
SHARED_OUT=${OUT}/shared
STATIC_OUT=${OUT}/static
SHARED_LIB=${SHARED_OUT}/${LIB_NAME}.so
STATIC_LIB=${STATIC_OUT}/${LIB_NAME}.a

DISCARD=$(OBJECTS) $(EXE) $(wildcard ${OUT}/**/*)

# check and see if we have ccache
HAS_CCACHE := $(shell command -v ccache 2> /dev/null)
ifdef HAS_CCACHE
	CC := ccache ${CC}
else
	CC := ${CC}
endif

.PHONY : clean tst run install profile

$(shell mkdir -p ${STATIC_OUT})
$(shell mkdir -p ${SHARED_OUT})

all : lib arc exe

lib : ${SHARED_LIB}

arc : ${STATIC_LIB}

exe : ${EXE}

${EXE} : ${OBJECTS}
	${CC} -o $@ $^ ${CFLAGS} ${LDFLAGS}

${LIB_OBJECTS} : ${LIB_SOURCES}
	${CC} -o $@ -fpic -c $^ $(CFLAGS) $(LDFLAGS)

${SHARED_LIB} : ${LIB_OBJECTS}
	${CC} -shared -o $@ $^

${STATIC_LIB} : ${LIB_OBJECTS}
	ar rcs $@ $^

ex :
	make -C examples all

install : install-exe install-lib

uninstall : uninstall-exe uninstall-lib

install-exe :
	install -s ${EXE} /usr/local/bin/${NAME}

uninstall-exe :
	rm /usr/local/bin/${NAME}
	
install-lib :
	cp ${SHARED_LIB} /usr/local/lib/${LIB_NAME}.so

uninstall-lib :
	rm /usr/local/lib/${NAME}.so

run :
	./pfmt -x 10 -y 10 -b ff0000 -f 00ff00 "Hello!"

clean :
	${RM} ${DISCARD}
	make -C examples clean

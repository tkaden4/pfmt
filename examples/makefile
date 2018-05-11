INCLUDE_DIR=../include/
LIB_DIR=../lib/static/
LIB=pfmt
CFLAGS:=${CFLAGS} -O0 -L${LIB_DIR} -l${LIB} -I${INCLUDE_DIR}

all: color position

color: color.c
	${CC} -o $@ $^ ${CFLAGS}

position: position.c
	${CC} -o $@ $^ ${CFLAGS}

clean:
	rm position color

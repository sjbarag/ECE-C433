.PHONY: clean
CC = gcc
CFLAGS1 = -g -lnsl
SRC1C = incrementClient.c
BIN1C = incrementClient
SRC1S = incrementServer.c
BIN1S = incrementServer

SRC2C = yadac.c
BIN2C = yadac
BINDIR  = bin/

default : q1

1c: ${SRC1C} ${BINDIR}
	gcc ${CFLAGS1} -o ${BINDIR}${BIN1C} ${SRC1C}

1s: ${SRS1S} ${BINDIR}
	gcc ${SFLAGS1} -o ${BINDIR}${BIN1S} ${SRC1S}

run1c:
	./${BINDIR}${BIN1C}

run1s:
	./${BINDIR}${BIN1S}

yadac:
	gcc ${FLAGS2} -o ${BINDIR}${BIN2C} ${SRC2C}

${BINDIR}:
	mkdir ${BINDIR}

clean :
	-rm ${BINDIR}/*

.PHONY: clean
CC = gcc
CFLAGS1 = -g -lnsl
SRC1C = incrementClient.c
OUT1C = incrementClient
SRC1S = incrementServer.c
OUT1S = incrementServer
OUTDIR  = out/

default : q1

1c: ${SRC1C} ${OUTDIR}
	gcc ${CFLAGS1} -o ${OUTDIR}${OUT1C} ${SRC1C}

1s: ${SRS1S} ${OUTDIR}
	gcc ${SFLAGS1} -o ${OUTDIR}${OUT1S} ${SRC1S}

run1c:
	./${OUTDIR}${OUT1C}

run1s:
	./${OUTDIR}${OUT1S}

${OUTDIR}:
	mkdir data/
	mkdir out/

clean :
	-rm ${OUTDIR}/*

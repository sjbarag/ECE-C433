.PHONY: clean
CC = gcc
CFLAGS  = -lpcap -g
OUTDIR  = out/
SRC_1A  = p1a.c
SRC_1B  = p1b.c
SRC_2   = src/queue_utils.c
HDR_1   = src/work_crew.h
HDR_2   = src/queue.h
OUTDIR  = out

default : q1

p1a : ${OUTDIR} p1a.c
	${CC} ${CFLAGS} -o ${OUTDIR}/p1a p1a.c

p1b : ${OUTDIR} p1b.c
	${CC} ${CFLAGS} -o ${OUTDIR}/p1b p1b.c

p1c : ${OUTDIR} p1c.c p1c.h
	${CC} ${CFLAGS} -lpthread -o ${OUTDIR}/p1c p1c.c

p1d : ${OUTDIR} p1d.c p1c.h
	${CC} ${CFLAGS} -lpthread -o ${OUTDIR}/p1d p1d.c

p2a : ${OUTDIR} p2a.c p1c.h
	${CC} ${CFLAGS} -lpthread -o ${OUTDIR}/p2a p2a.c

run all:
	echo "Running all, yo"

run1:
	echo "Running p1a"

run2:
	echo "Running p1b"

${OUTDIR}:
	mkdir data/
	mkdir out/

clean :
	-rm ${OUTDIR}/*

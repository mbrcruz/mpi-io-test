MPI_DIR=/opt/mpich2/mpich-4.2.1-gcc
SRCS=test.c
OBJECTS=$(SRCS:.c=.o)
CFLAGS=-I ${MPI_DIR}/include   -c
LDFLAGS= -L ${MPI_DIR}/lib -lmpi
CC=gcc

all: prog





prog: ${OBJECTS}
        ${CC} ${LDFLAGS} ${OBJECTS} -o $@

.c.o:
        ${CC} ${CFLAGS} $<
clean:
        @rm -f *.o pro
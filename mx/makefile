CFLAGS = -O -lm

SRCS =   mx.c \
		mx.readit.c

BSRC = mxbitmap.c \
		mx.readit.c

OBJS =  mxbitmap.o \
		mx.readit.o

BOBJS = mxbitmap.o \
		mx.readit.o


mx: ${OBJS}
	cc ${OBJS} -o mx ${CFLAGS}

mxbm: ${BOBJS}
	cc ${BOBJS} -o mx ${CFLAGS}

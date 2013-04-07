CFLAGS = -L/usr/X11R6/lib -lX11

CC = gcc

OUTFILE = xmx

SRCS =   xmx.c \
		xmxfract.c

OBJS =  xmx.o \
		xmxfract.o

all: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} -o ${OUTFILE} 

xmx.o: xmx.c
	${CC} -c xmx.c

xmxfract.o: xmxfract.c
	${CC} -c xmxfract.c

lint:
	lint -h ${SRCS}

clean:
	rm -f core *.o

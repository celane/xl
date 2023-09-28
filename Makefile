# uncomment next line if you need shadow passwd support
#NEEDSHADOW= -DNEED_SHADOW

VERSION = 1.0
X11HOME = /usr/X11R6

CC	= gcc
CFLAGS	= -O2 -Wall -I${X11HOME}/include ${NEEDSHADOW}
LIBS	= -L${X11HOME}/lib -lX11 -lpam


# Uncomment for systems with libcrypt
LIBS   += -lcrypt

# uncomment if you are running under solaris
#LIBS   += -lnsl -lsocket

OBJS = main.o

all:	xl

xl: $(OBJS)
	$(CC) $(CFLAGS) -o xl $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS)

realclean: clean
	rm -f xl

dist: LICENSE  main.c  Makefile  README.md  xl.spec
	tar czvf xl-$(VERSION).tgz $^

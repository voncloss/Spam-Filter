
SHELL = /bin/sh

CC = gcc
CFLAGS = -fPIC
LFLAGS = -shared
TARGET = libsf.so
SOURCES = $(shell echo *.c)
HEADERS = $(shell echo *.h)
OBJECTS = $(SOURCES:.c=.o)

INSTALLPATH = /usr/local/lib/$(TARGET)

install:
	cp -p $(TARGET) $(INSTALLPATH)

lib: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $(TARGET) $(OBJECTS)

.c.o:
	${CC} ${CFLAGS} -c $<

CC=gcc
CFLAGS=-Wall -Wextra -lpng -I/usr/include -L/usr/lib64 -Wl,-rpath -Wl,/usr/lib64 -lmpich -lopa -lmpl -lgmp
CFLAGSL=-lpng
LDFLAGS=

SOURCES=mandel_processor.c# mandel.c main.c image_utils.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=magic

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(CFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o -c $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)
	$(RM) *.ppm

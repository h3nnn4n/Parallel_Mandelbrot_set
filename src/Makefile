CC=gcc
CFLAGS=-Wall -Wextra -g -lpng -fopenmp -lm

SOURCES=mandel.c image_utils.c mandel_processor.c main.c color.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -c -o $@

.PHONY: clean
clean:
	$(RM) $(OBJECTS) $(EXECUTABLE)

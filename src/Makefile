CC=gcc
CFLAGS=-c -Wall -Wextra -g -lpng
CFLAGSL=-lpng -fopenmp -pthread
LDFLAGS=
PTHREAD_FLAG=-pthread
OPENMP_FLAG=-fopenmp

SOURCES=mandel.c image_utils.c mandel_processor.c
EXECUTABLES=main main_pthreads main_openmp
ALLSOURCES=mandel.c image_utils.c main.c main_pthreads.c main_openmp.c mandel_processor.c

OBJECTS=$(SOURCES:.c=.o)
ALLOBJECTS=$(ALLSOURCES:.c=.o)

all: main main_pthreads main_openmp

image_utils.c:
	$(CC) $(CFLAGS) $< -o $@ $(CFLAGSL)

mandel.c:
	$(CC) $(CFLAGS) $< -o $@ $(CFLAGSL)

main.c: $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@ $(CFLAGSL)

main_pthreads.c: $(SOURCES)
	$(CC) $(CFLAGS) $< -o $@ $(CFLAGSL) $(PTHREAD_FLAG)

main_openmp.c: $(SOURCES)
	$(CC) $(CFLAGS) $(OPENMP_FLAG) $< -o $@ $(CFLAGSL)

main: $(ALLOBJECTS)
	$(CC) main.o $(LDFLAGS) $(OBJECTS) -o $@ $(CFLAGSL)

main_pthreads: $(ALLOBJECTS)
	$(CC) main_pthreads.o $(LDFLAGS) $(OBJECTS) -o $@ $(CFLAGSL) $(PTHREAD_FLAG)

main_openmp: $(ALLOBJECTS)
	$(CC) main_openmp.o $(LDFLAGS) $(OBJECTS) -o $@ $(CFLAGSL) $(OPENMP_FLAG)

.PHONY: clean
clean:
	$(RM) $(ALLOBJECTS) $(EXECUTABLES)

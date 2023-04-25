CC = gcc
CFLAGS = -O3 -fopenmp

simple_loop_serial: simple_loop_serial.o
	$(CC) $(CFLAGS) -o simple_loop_serial simple_loop_serial.o -lm
	rm simple_loop_serial.o

simple_loop_omp: simple_loop_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o simple_loop_omp simple_loop_omp.o -lm
	rm simple_loop_omp.o

reduction_loop_omp: reduction_loop_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o reduction_loop_omp reduction_loop_omp.o -lm
	rm reduction_loop_omp.o

reduction_loop2_omp: reduction_loop2_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o reduction_loop2_omp reduction_loop2_omp.o -lm
	rm reduction_loop2_omp.o

all: simple_loop_serial simple_loop_omp reduction_loop_omp reduction_loop2_omp

clean:
	rm -f *.o simple_loop_serial simple_loop_omp reduction_loop_omp reduction_loop2_omp

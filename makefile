CC = gcc
CFLAGS = -O3 -fopenmp

simple_loop_serial: simple_loop_serial.o
	$(CC) $(CFLAGS) -o simple_loop_serial simple_loop_serial.o -lm

simple_loop_omp: simple_loop_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o simple_loop_omp simple_loop_omp.o -lm

reduction_loop_omp: reduction_loop_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o reduction_loop_omp reduction_loop_omp.o -lm

reduction_loop2_omp: reduction_loop2_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o reduction_loop2_omp reduction_loop2_omp.o -lm

nested_loops_omp: nested_loops_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o nested_loops_omp nested_loops_omp.o -lm

collapse_omp: collapse_omp.o
	$(CC) $(CFLAGS) $(OMPFLAGS) -o collapse_omp collapse_omp.o -lm

all: simple_loop_serial simple_loop_omp reduction_loop_omp reduction_loop2_omp nested_loops_omp collapse_omp

clean:
	rm -f *.o simple_loop_serial simple_loop_omp reduction_loop_omp reduction_loop2_omp nested_loops_omp collapse_omp

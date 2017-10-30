all:main time 
main:main.o
	mpicc -o main main.o -lm -g -Wall 
main.o:main.c
	mpicc -c main.c
time:time.o
	mpicc -o time time.o -lm -g -Wall 
time.o:time.c
	mpicc -c time.c
clean:
	rm -f all main time *.o

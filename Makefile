main:main.c
	mpicc  main.c -o main -lm
clean:
	rm -f main *.o

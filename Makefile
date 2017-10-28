main:main.c
	mpicc  main.c -o main
clean:
	rm -f main *.o

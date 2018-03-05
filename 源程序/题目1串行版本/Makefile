all:mainchuanxing timechuanxing 
mainchuanxing:mainchuanxing.o
	mpicc -O2 -o mainchuanxing mainchuanxing.o -lm -Wall 
mainchuanxing.o:mainchuanxing.c
	mpicc -O2 -c mainchuanxing.c -lm -Wall
timechuanxing:timechuanxing.o
	mpicc -O2 -o timechuanxing timechuanxing.o -lm -Wall 
timechuanxing.o:timechuanxing.c
	mpicc -O2 -c timechuanxing.c -lm -Wall
clean:
	rm -f all mainchuanxing timechuanxing *.o

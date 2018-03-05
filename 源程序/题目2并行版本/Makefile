all:main time 
main:main.cc block.cc
	mpicxx -O2 -o main main.cc block.cc -lm -Wall 
time:time.cc
	mpicxx -O2 -o time time.cc -lm -Wall 
clean:
	rm -f all main time

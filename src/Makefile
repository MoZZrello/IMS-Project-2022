CC = g++
FLAGS = -g -O2 -o
BASE_FILES = main.cpp main.h
SIMLIB = -lsimlib -lm
OUTPUT = solar
DATA = solar.dat

.PHONY: all run clean

all: $(BASE_FILES)
	$(CC) $(FLAGS) $(OUTPUT) $(BASE_FILES) $(SIMLIB)

run: all
	./$(OUTPUT) 3.6 150000 3000 0 8 0.244 5000 4000 exp1.dat
	./$(OUTPUT) 3.6 150000 13000 0 6.5 0.244 5000 5000 exp2.dat
	./$(OUTPUT) 6.45 300000 13000 0 6.5 0.244 5000 5000 exp3.dat
	./$(OUTPUT) 3.6 150000 13000 5 6.5 0.244 5000 5000 exp4.dat
	./$(OUTPUT) 6.45 300000 13000 5 6.5 0.244 5000 5000 exp5.dat
	./$(OUTPUT) 3.6 150000 13000 10 6.5 0.244 5000 5000 exp6.dat
	./$(OUTPUT) 6.45 300000 13000 10 6.5 0.244 5000 5000 exp7.dat

clean:
	rm $(OUTPUT) $(DAT) exp1.dat
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
	./$(OUTPUT) 3.6 100 150000 4.8 3000 8 0.244 5000 4000 exp1.dat

clean:
	rm $(OUTPUT) $(DAT) exp1.dat
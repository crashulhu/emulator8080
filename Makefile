OUT=./emulator8080
IN=main.c 
SRC=emulate8080.c opcodeHandlers.c
FLAGS=
CC=gcc
TEST_RUNNER=./runner.c


all:
	$(CC) $(IN) $(SRC) $(FLAGS) -o $(OUT)

clean:

	if [ -f "./runner" ]; then \
		rm ./runner; \
	fi 

	if [ -f "$(OUT)" ]; then \
		rm $(OUT); \
	fi 



runner:
	$(CC) $(TEST_RUNNER) $(SRC) $(FLAGS) -o ./runner

run_tests: clean runner 
	./runner

run:
	$(OUT) ../invaders


CC=g++
DEBUG=-g
FILE=main
LIB=camellia.cpp
OPTION=-O3
BIN=camellia

build:
	$(CC) $(FILE).cpp $(LIB) $(OPTION) -o ../$(BIN)

debug:
	$(CC) $(DEBUG) $(FILE).cpp $(LIB) -o ../$(BIN)

test:
	@$(CC) test.cpp $(LIB) -o ../camellia-test
	@echo "Run test from RFC"
	@../camellia-test
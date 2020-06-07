
SRC=./src
INC=./include
OBJ=./obj
BIN=./bin

CC=g++

all: $(BIN)/main.o

$(BIN)/main.o: $(SRC)/main.cpp $(OBJ)/save_utils.o
	$(CC) \
    -I/usr/local/include \
	$(OBJ)/save_utils.o \
	-o $@ $<

$(OBJ)/save_utils.o: $(SRC)/save_utils.cpp $(INC)/save_utils.h
	$(CC) -c \
	-I/usr/local/include \
	-o $@ $<

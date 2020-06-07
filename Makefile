SRC=./src
INC=./include
OBJ=./obj
BIN=./bin

CC=g++ -std=c++17

all: $(BIN)/main.o

$(BIN)/main.o: $(SRC)/main.cpp $(OBJ)/save_utils.o $(OBJ)/Intersectables.o $(OBJ)/SimpleLights.o $(INC)/Raytracing.h $(INC)/SimpleMaterial.h
	$(CC) \
    -I/usr/local/include \
	$(OBJ)/Intersectables.o \
	$(OBJ)/SimpleLights.o \
	$(OBJ)/save_utils.o \
	-o $@ $<

$(OBJ)/%.o: $(SRC)/%.cpp $(INC)/%.h $(INC)/Raytracing.h
	$(CC) -c \
	-I/usr/local/include \
	-o $@ $<

.PHONY clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*

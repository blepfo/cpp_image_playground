SRC=./src
INC=./include
OBJ=./obj
BIN=./bin

CC=g++ -std=c++17


all: $(BIN)/main.o

$(BIN)/main.o: $(SRC)/main.cpp $(OBJ)/save_utils.o $(OBJ)/SimpleObjects.o $(OBJ)/SimpleLights.o $(OBJ)/SimpleScene.o $(OBJ)/Transform.o $(INC)/SimpleMaterial.h
	$(info $@)
	$(CC) \
    -I/usr/local/include \
	$(OBJ)/SimpleLights.o \
	$(OBJ)/SimpleObjects.o \
    $(OBJ)/SimpleScene.o \
	$(OBJ)/save_utils.o \
    $(OBJ)/Transform.o \
	-o $@ $<

$(OBJ)/SimpleLights.o: $(SRC)/SimpleLights.cpp $(INC)/SimpleLights.h $(INC)/SimpleMaterial.h
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-o $@ $<

$(OBJ)/SimpleObjects.o: $(SRC)/SimpleObjects.cpp $(INC)/SimpleObjects.h $(INC)/Raytracing.h
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-o $@ $<

$(OBJ)/SimpleScene.o: $(SRC)/SimpleScene.cpp $(INC)/SimpleScene.h $(INC)/SimpleMaterial.h $(INC)/SimpleLights.h $(INC)/Raytracing.h
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-o $@ $<

$(OBJ)/%.o: $(SRC)/%.cpp $(INC)/%.h
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-o $@ $<


.PHONY clean:
	rm $(OBJ)/*.o
	rm $(BIN)/*

SRC=./src
INC=./include
BUILD=./build
BIN=./bin


SG=SimpleGraphics
SG_SRC=$(SG)/src
SG_BUILD=$(SG)/build


CC=g++ -std=c++17


all: $(BIN)/main.o

$(BIN)/main.o: $(SRC)/main.cpp $(BUILD)/PixelDraw.o $(BUILD)/SaveUtils.o $(BUILD)/Transform.o $(SG_BUILD)/Camera.o $(SG_BUILD)/Lights.o $(SG_BUILD)/Objects.o $(SG_BUILD)/Scene.o $(SG)/Materials.hpp ./Raytracing.hpp
	$(info $@)
	$(CC) \
	-lomp \
	-I/usr/local/include \
	-I./ \
	$(SG_BUILD)/Camera.o \
	$(SG_BUILD)/Lights.o \
	$(SG_BUILD)/Objects.o \
	$(SG_BUILD)/Scene.o \
	$(BUILD)/PixelDraw.o \
	$(BUILD)/SaveUtils.o \
	$(BUILD)/Transform.o \
	-o $@ $<

$(SG_BUILD)/Camera.o: $(SG_SRC)/Camera.cpp $(SG)/Camera.hpp ./Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I./ \
	-o $@ $<

$(SG_BUILD)/Lights.o: $(SG_SRC)/Lights.cpp $(SG)/Lights.hpp $(SG)/Materials.hpp ./Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I./ \
	-o $@ $<

$(SG_BUILD)/Objects.o: $(SG_SRC)/Objects.cpp $(SG)/Objects.hpp ./Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I./ \
	-o $@ $<

$(SG_BUILD)/Scene.o: $(SG_SRC)/Scene.cpp $(SG)/Scene.hpp $(SG)/Lights.hpp $(SG)/Materials.hpp $(SG)/Objects.hpp ./Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I./ \
	-o $@ $<

$(BUILD)/PixelDraw.o: $(SRC)/PixelDraw.cpp ./PixelDraw.hpp
	$(info $@)
	$(CC) -c \
	-Xpreprocessor -fopenmp \
	-I/usr/local/include \
	-I./ \
	-o $@ $<

$(BUILD)/%.o: $(SRC)/%.cpp ./%.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I./ \
	-o $@ $<


.PHONY clean:
	rm $(BUILD)/*.o
	rm $(BIN)/*

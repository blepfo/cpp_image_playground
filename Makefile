BIN=./bin
BUILD=./build
LIB=./lib
SRC=./src

MAKEFILE_DIR=$(dir $(abspath $(firstword $(MAKEFILE_LIST))))

SG=$(MAKEFILE_DIR)SimpleGraphics
SG_SRC=$(SG)/src
SG_BUILD=$(BUILD)/SimpleGraphics


CC=g++ -std=c++17

all: directories $(BIN)/main.o

$(BIN)/main.o: $(SRC)/main.cpp $(LIB)/CppImage.a $(LIB)/SimpleGraphics.a 
	$(info $@)
	$(CC) \
	-lomp \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	$(LIB)/CppImage.a \
	$(LIB)/SimpleGraphics.a \
	-o $@ \
	$(SRC)/main.cpp

# Object archives for top level CppImage + SimpleGraphics
$(LIB)/CppImage.a: $(BUILD)/PixelDraw.o  $(BUILD)/SaveUtils.o $(BUILD)/Transform.o $(MAKEFILE_DIR)Raytracing.hpp
	$(info $@)
	ar rvs $@ $(filter %.o, $^)

$(LIB)/SimpleGraphics.a: $(SG_BUILD)/Camera.o $(SG_BUILD)/Lights.o $(SG_BUILD)/Objects.o $(SG_BUILD)/Scene.o
	$(info $@)
	ar rvs $@ $(filter %.o, $^)


# SimpleGraphics object files
$(SG_BUILD)/Camera.o: $(SG_SRC)/Camera.cpp $(SG)/Camera.hpp $(MAKEFILE_DIR)Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-o $@ $<

$(SG_BUILD)/Lights.o: $(SG_SRC)/Lights.cpp $(SG)/Lights.hpp $(SG)/Materials.hpp $(MAKEFILE_DIR)Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-o $@ $<

$(SG_BUILD)/Objects.o: $(SG_SRC)/Objects.cpp $(SG)/Objects.hpp $(MAKEFILE_DIR)Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-o $@ $<

$(SG_BUILD)/Scene.o: $(SG_SRC)/Scene.cpp $(SG)/Scene.hpp $(SG)/Lights.hpp $(SG)/Materials.hpp $(SG)/Objects.hpp $(MAKEFILE_DIR)Raytracing.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-o $@ $<


# CppImage object files
$(BUILD)/PixelDraw.o: $(SRC)/PixelDraw.cpp $(MAKEFILE_DIR)PixelDraw.hpp
	$(info $@)
	$(CC) -c \
	-Xpreprocessor -fopenmp \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-o $@ $<

$(BUILD)/%.o: $(SRC)/%.cpp ./%.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-o $@ $<


directories: $(BUILD) $(SG_BUILD) $(BIN) $(LIB)

$(BUILD):
	mkdir -p $@

$(SG_BUILD):
	mkdir -p $@

$(BIN):
	mkdir -p $@

$(LIB):
	mkdir -p $@


.PHONY clean:
	rm -rf $(BUILD) $(LIB) $(BIN)

BIN=./bin
BUILD=./build
LIB=./lib
SRC=./src

MAKEFILE_DIR=$(dir $(abspath $(firstword $(MAKEFILE_LIST))))

SG=$(MAKEFILE_DIR)SimpleGraphics
SG_SRC=$(SG)/src
SG_BUILD=$(BUILD)/SimpleGraphics

# Location of FreeImage install
FREEIMAGE_DIST=/usr/local/include/FreeImage/Dist


CC=g++ -std=c++17
CPP_ARGS=-Wall

all: directories demo


# Demos
demo: directories demo_shinyball

demo_shinyball: $(BIN)/demo_shinyball

$(BIN)/demo_shinyball: demo_shinyball/main.cpp $(LIB)/CppImage.a $(LIB)/SimpleGraphics.a
	$(info $@)
	$(CC) $(CPP_ARGS) \
	-lomp \
	-I/usr/local/include \
	-I$(MAKEFILE_DIR) \
	-I$(FREEIMAGE_DIST) \
	-L$(FREEIMAGE_DIST) -lfreeimage \
	$(LIB)/CppImage.a \
	$(LIB)/SimpleGraphics.a \
	-o $@ \
	$(filter %.cpp %.a, $^)


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

$(BUILD)/SaveUtils.o: $(SRC)/SaveUtils.cpp $(MAKEFILE_DIR)SaveUtils.hpp
	$(info $@)
	$(CC) -c \
	-I/usr/local/include \
	-I$(FREEIMAGE_DIST) \
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

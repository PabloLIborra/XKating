#Makefile for X-Kating Project

# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := .

# Our .cpp and where to store them
SOURCES:= $(wildcard *.cpp)
_OBJECTS := $(subst .cpp,.o,$(SOURCES))
BIN_DIR := ../../../bin/
OBJ_DIR := obj/
OBJECTS := $(patsubst %,$(BIN_DIR)$(OBJ_DIR)%,$(_OBJECTS))
NDEBUG := ''
# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I./../../../ -I/usr/X11R6/include -I./../../../include
ifndef NDEBUG
CXXFLAGS += -g -Wall -std=c++14
else
CXXFLAGS += -O2
endif

#default target is Linux
all: all_linux

all_linux: $(OBJECTS)
	$(warning Compilation done!)

$(BIN_DIR)$(OBJ_DIR)%.o: %.cpp
	$(warning Compiling...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $^

clean: clean_linux
	$(warning Cleaning...)

clean_linux:
	rm -r $(BIN_DIR)$(OBJ_DIR)

info:
		$(info $(SOURCES))
		$(info $(OBJECTS))

.PHONY: all clean clean_linux make_dir info 

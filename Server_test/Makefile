 
#Makefile for the test server

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := XKating
TARGET_SERVER := Server_XKating

# Path for the executable. Note that Irrlicht.dll should usually also be there for win32 systems
BIN_PATH := bin/
OBJ_PATH := obj/


# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I$(IrrlichtHome)/include -I/usr/X11R6/include -I.
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O3
endif

# Instruction to create directories
MKDIR_P = mkdir -p

#default target is Linux
all: create_dir make_src all_linux
	$(warning Linking done!)

create_dir: make_bin_dir make_obj_dir

make_bin_dir:
	$(warning Creating bin directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_PATH)

make_obj_dir:
	$(warning Creating obj directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_PATH)$(OBJ_PATH)

# target specific settings
all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
all_linux: LDFLAGS += -L./lib/linux/irrlicht -lIrrlicht -Wl,-rpath=./lib/linux/irrlicht
all_linux: LDFLAGS += -L./lib/linux/fmod -lfmod -lfmodstudio -Wl,-rpath=./lib/linux/fmod
all_linux: LDFLAGS += -L./lib/linux/raknet -lRakNetDºLL -Wl,-rpath=./lib/linux/raknet

all_linux:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -r $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

server:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGETSERVER) $(LDFLAGS)

################################################################################
# Windows 
################################################################################
all_windows: LDFLAGS += -L./lib/win -lIrrlicht -lopengl32 -lm #-lfmod -lfmodstudio
all_windows:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)



.PHONY: all clean clean_linux info install win

#multilib handling
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif

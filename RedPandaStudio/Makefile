#Makefile for RedPandaStudio Project

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := libRedPanda

# Path for the executable
BIN_PATH := bin/
OBJ_PATH := obj/

NDEBUG := ''
# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I.
ifndef NDEBUG
CXXFLAGS += -g -Wall
else
CXXFLAGS += -O2
endif

# Instruction to create directories
MKDIR_P = mkdir -p

#default target is Linux
all: create_dir make_src make_fac all_linux
	$(warning Linking done!)

#we can also have Windows
win: create_dir make_src make_fac all_windows
	$(warning Linking done!)

mac: create_dir make_src make_fac all_mac
	$(warning Linking done!)

create_dir: make_bin_dir make_obj_dir

make_bin_dir:
	$(warning Creating bin directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_PATH)

make_obj_dir:
	$(warning Creating obj directory... (if it doesn't exist))
	$(MKDIR_P) $(BIN_PATH)$(OBJ_PATH)

#Makes to execute
make_src:
	$(MAKE) -C src

make_fac:
	$(MAKE) -C src/Facade

# target specific settings
all_linux: LDFLAGS += -lGLEW -lGLU -lGL
all_linux: LDFLAGS += -L./lib/assimp -lassimp -Wl,-rpath=lib/assimp
all_linux: LDFLAGS += -L./lib/sdl -lSDL2 -Wl,-rpath=lib/sdl
all_linux: LDFLAGS += -L./lib/sfml -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath=lib/sfml

all_linux:
	$(CXX) $(CXXFLAGS) -shared -o $(BIN_PATH)$(TARGET).so $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) $(LDFLAGS)

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -rf $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

################################################################################
# Windows 
################################################################################
all_windows: LDFLAGS += -L./lib/win -lopengl32 -lmingw32 -lSDL2 -lglew32 -lassimp -lsfml-graphics-2 -lsfml-system-2
all_windows:
	$(CXX) $(CXXFLAGS) -shared -o $(BIN_PATH)$(TARGET).dll $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) $(LDFLAGS)

################################################################################
# MAC
################################################################################
all_mac: LDFLAGS += -L./lib/mac -lSDL2 -lGLEW -lassimp -lsfml-graphics -lsfml-system
all_mac: LDFLAGS += -dylib_file \
/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:\
/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib \
-framework Cocoa -framework OpenGL -framework Carbon -framework IOKit 
all_mac:
	$(CXX) $(CXXFLAGS) -dynamiclib -undefined suppress -flat_namespace -o $(BIN_PATH)$(TARGET).dylib $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) $(LDFLAGS)

hashtag := "\#"
newline := \"\\n
includeCommand := $(hashtag)include \"
sh_value := "export LD_LIBRARY_PATH=./bin/:lib/sfml:lib/assimp:lib/sdl && ./bin/test"

compile: LDFLAGS += -lGLEW -lGLU -lGL
compile: LDFLAGS += -L./lib/sfml -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath=lib/sfml
compile: LDFLAGS += -L./lib/assimp -lassimp -Wl,-rpath=lib/assimp
compile: LDFLAGS += -L./lib/sdl -lSDL2 -Wl,-rpath=lib/sdl
compile: LDFLAGS += -L./bin -lRedPanda -Wl,-rpath=./bin

compilel: LDFLAGS += -lGLEW -lGLU -lGL
compilel: LDFLAGS += -L./lib/sfml -lsfml-graphics -lsfml-window -lsfml-system -Wl,-rpath=lib/sfml
compilel: LDFLAGS += -L./lib/assimp -lassimp -Wl,-rpath=lib/assimp
compilel: LDFLAGS += -L./lib/sdl -lSDL2 -Wl,-rpath=lib/sdl
compilel: LDFLAGS += -L./bin -lRedPanda -Wl,-rpath=./bin

compilelwin: LDFLAGS += -L./lib/win -lopengl32 -lmingw32 -lSDL2 -lglew32 -lassimp -lsfml-graphics-2 -lsfml-system-2 -L./bin -lRedPanda

compilelmac: LDFLAGS += -L./lib/mac -lSDL2 -lGLEW -lassimp -lsfml-graphics -lsfml-system -L./bin -lRedPanda
compilelmac: LDFLAGS += -dylib_file \
/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:\
/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib \
-framework Cocoa -framework OpenGL -framework Carbon -framework IOKit

test: remdir makedir copyfiles includefiles compile makesh
	$(warning Done!)
	
testl: remdir makedir copyfiles includefiles compilel makesh

testlwin: remdir makedir copyfiles includefiles compilelwin makesh

testlmac: remdir makedir copyfiles includefilesmac compilelmac makeshmac

remdir:
	rm -rf ./inc

makedir:
	mkdir -p ./inc

copyfiles:
	cp -r ./src/*.h ./inc
	#cp -r ./src/Facade/*.h ./inc

includefiles:
	@echo -e $(addprefix $(includeCommand), $(addsuffix $(newline),$(shell ls ./inc))) >> ./inc/RedPanda.h

includefilesmac:
	@echo $(addprefix $(includeCommand), $(addsuffix $(newline),$(shell ls ./inc))) >> ./inc/RedPanda.h
	
compile:
	g++ -I./inc -c -o bin/main.o main.cpp
	g++ -o bin/test bin/main.o $(LDFLAGS)

compilel:
	g++ -I./inc -c -o bin/main.o main.cpp
	g++ -o bin/test bin/main.o $(LDFLAGS)

compilelwin:
	g++ -I./inc -I./include -c -o bin/main.o main.cpp
	g++ -o bin/test bin/main.o $(LDFLAGS)

compilelmac:
	$(CXX) -I./inc -I./include -c -o bin/main.o main.cpp
	$(CXX) -o bin/test bin/main.o $(LDFLAGS)

makesh:
	rm -rf ./bin/test.sh
	@echo -e $(sh_value) >> ./bin/test.sh
	chmod +x ./bin/test.sh

sh_valueMAC := "export LD_LIBRARY_PATH=lib/mac/:bin/ && bin/test"

makeshmac:
	rm -rf ./bin/test.sh
	@echo $(sh_valueMAC) >> ./bin/test.sh
	chmod +x ./bin/test.sh

################################################################################
# To update GraphicsEngine in  the Game
################################################################################
port: remdirP makedirP copyfilesP compileP
	$(warning Done!)

portwin: remdirP makedirP copyfilesP compilePwin

portmac: remdirP makedirPmac copyfilesPmac compilePmac

remdirP:
	rm -rf ../Game/include/redpanda

makedirP:
	mkdir -p ../Game/include/redpanda
	mkdir -p ../Game/shaders

makedirPmac:
	mkdir -p ../Game/include/redpanda
	mkdir -p ../Game/shaders
	mkdir -p ../Game/shaders/MAC

copyfilesP:
	cp -r ./inc/*.h ../Game/include/redpanda
	cp ./src/Facade/RedPandaStudio.h ../Game/include/redpanda
	cp ./shaders/*.vert	../Game/shaders/
	cp ./shaders/*.frag	../Game/shaders/

copyfilesPmac:
	cp -r ./inc/*.h ../Game/include/redpanda
	cp ./src/Facade/RedPandaStudio.h ../Game/include/redpanda
	cp ./shaders/*.vert	../Game/shaders/
	cp ./shaders/*.frag	../Game/shaders/
	cp ./shaders/MAC/*.vert	../Game/shaders/MAC
	cp ./shaders/MAC/*.frag	../Game/shaders/MAC

compileP: all 
	cp ./bin/libRedPanda.so ../Game/lib/linux

compilePwin: win
	cp ./bin/libRedPanda.dll ../Game/lib/win
	cp ./bin/libRedPanda.dll ../Game/bin

compilePmac: mac
	cp ./bin/libRedPanda.dylib ../Game/lib/mac
	cp ./bin/libRedPanda.dylib ../Game/bin

install_mac:
	brew reinstall sdl2 
	brew reinstall sfml 
	brew reinstall assimp 
	brew reinstall glew
	
.PHONY: all clean clean_linux info install win
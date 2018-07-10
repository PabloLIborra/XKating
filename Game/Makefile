#Makefile for X-Kating Project

# Name of the executable created (.exe will be added automatically if necessary)
TARGET := XKating
TARGET_SERVER := Server_XKating

# Path for the executable
BIN_PATH := bin/
OBJ_PATH := obj/

NDEBUG := ''
# general compiler settings (might need to be set when compiling the lib, too)
CPPFLAGS += -I$(IrrlichtHome)/include -I/usr/X11R6/include -I.
ifndef NDEBUG
CXXFLAGS += -g -Wall -std=c++14
else
CXXFLAGS += -O2
endif

# Instruction to create directories
MKDIR_P = mkdir -p

#default target is Linux
all: create_dir make_client make_src make_gameF make_gameM make_gameS make_gameO make_gameO_R make_gameO_A make_gameO_I make_gameO_F make_gameO_AU make_gameO_N make_gameE make_gameP make_gameB make_gameA all_linux
	$(warning Linking done!)

server: create_dir make_server make_src make_gameF make_gameM make_gameS make_gameO make_gameO_R make_gameO_A make_gameO_I make_gameO_F make_gameO_AU make_gameO_N make_gameE make_gameP make_gameB make_gameA all_linux_server
	$(warning Linking done!)

#we can also have Windows
win: create_dir make_client make_src make_gameF make_gameM make_gameS make_gameO make_gameO_R make_gameO_A make_gameO_I make_gameO_F make_gameO_AU make_gameO_N make_gameE make_gameP make_gameB make_gameA all_windows
	$(warning Linking done!)

#we can also have Mac
mac: create_dir make_client make_src make_gameF make_gameM make_gameS make_gameO make_gameO_R make_gameO_A make_gameO_I make_gameO_F make_gameO_AU make_gameO_N make_gameE make_gameP make_gameB make_gameA all_mac
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

make_client:
	g++ -I./../ -I/usr/X11R6/include -I./include -g -Wall -std=c++14 -c -o bin/obj/main.o src/GameClient/main.cpp
	$(MAKE) -C src/GameClient

make_server:
	g++ -I./../ -I/usr/X11R6/include -I./include -g -Wall -std=c++14 -c -o bin/obj/main.o src/GameServer/main.cpp
	$(MAKE) -C src/GameServer
	
make_gameF:
	$(MAKE) -C src/GameFacade

make_gameM:
	$(MAKE) -C src/GameManager

make_gameS:
	$(MAKE) -C src/GameState

make_gameO:
	$(MAKE) -C src/GameObject

make_gameO_R:
	$(MAKE) -C src/GameObject/RenderComponent

make_gameO_A:
	$(MAKE) -C src/GameObject/AIComponent

make_gameO_I:
	$(MAKE) -C src/GameObject/ItemComponent
	
make_gameO_F:
	$(MAKE) -C src/GameObject/PhysicsComponent

make_gameO_AU:
	$(MAKE) -C src/GameObject/AudioComponent

make_gameO_N:
	$(MAKE) -C src/GameObject/NetworkComponent

make_gameE:
	$(MAKE) -C src/GameEvent
	
make_gameP:
	$(MAKE) -C src/GamePhysics

make_gameB:
	$(MAKE) -C src/GameBehaviourTrees

make_gameA:
	$(MAKE) -C src/GameAudio

# target specific settings
all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor -lGLEW -lGLU -lGL
all_linux: LDFLAGS += -L./lib/linux/irrlicht -lIrrlicht
all_linux: LDFLAGS += -L./lib/linux/fmod -lfmod -lfmodstudio
all_linux: LDFLAGS += -L./lib/linux/raknet -lRakNetDLL
all_linux: LDFLAGS += -L./lib/linux/sdl -lSDL2
all_linux: LDFLAGS += -L./lib/linux/assimp -lassimp
all_linux: LDFLAGS += -L./lib/linux/sfml -lsfml-graphics -lsfml-window -lsfml-system 
all_linux: LDFLAGS += -L./lib/linux -lRedPanda -lcsound64 -lcsnd6 -L./lib/linux/csound/plugins64-6.0
all_linux: LDFLAGS += -Wl,-rpath=lib/linux/irrlicht -Wl,-rpath=lib/linux/fmod -Wl,-rpath=lib/linux/raknet -Wl,-rpath=lib/linux/sdl -Wl,-rpath=lib/linux
all_linux: LDFLAGS += -Wl,-rpath=lib/linux/assimp -Wl,-rpath=lib/linux/sfml -Wl,-rpath=lib/linux/csound/plugins64-6.0

sh_value := "export LD_LIBRARY_PATH=lib/linux/fmod/:lib/linux/irrlicht/:lib/linux/raknet/:lib/linux/sdl:lib/linux:lib/linux/assimp:lib/linux/sfml && export OPCODE6DIR64=lib/linux/csound/plugins64-6.0 && bin/XKating"
sh_server_value := "export LD_LIBRARY_PATH=lib/linux/fmod/:lib/linux/irrlicht/:lib/linux/raknet/:lib/linux && export OPCODE6DIR64=lib/linux/csound/plugins64-6.0 && bin/Server_XKating"

all_linux:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)
	rm -rf $(BIN_PATH)$(TARGET).sh
	@echo -e $(sh_value) >> $(BIN_PATH)$(TARGET).sh
	chmod +x $(BIN_PATH)$(TARGET).sh	

all_linux_server: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor -lGLEW -lGLU -lGL
all_linux_server: LDFLAGS += -L./lib/linux/irrlicht -lIrrlicht
all_linux_server: LDFLAGS += -L./lib/linux/fmod -lfmod -lfmodstudio
all_linux_server: LDFLAGS += -L./lib/linux/raknet -lRakNetDLL
all_linux_server: LDFLAGS += -L./lib/linux/sdl -lSDL2
all_linux_server: LDFLAGS += -L./lib/linux/assimp -lassimp
all_linux_server: LDFLAGS += -L./lib/linux/sfml -lsfml-graphics -lsfml-window -lsfml-system 
all_linux_server: LDFLAGS += -L./lib/linux -lRedPanda
all_linux_server: LDFLAGS += -Wl,-rpath=lib/linux/irrlicht -Wl,-rpath=lib/linux/fmod -Wl,-rpath=lib/linux/raknet -Wl,-rpath=lib/linux/sdl -Wl,-rpath=lib/linux
all_linux_server: LDFLAGS += -Wl,-rpath=lib/linux/assimp -Wl,-rpath=lib/linux/sfml
all_linux_server: LDFLAGS += -L./lib/linux -lRedPanda -lcsound64 -lcsnd6

all_linux_server:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET_SERVER) $(LDFLAGS)
	rm -rf $(BIN_PATH)$(TARGET_SERVER).sh
	@echo -e $(sh_server_value) >> $(BIN_PATH)$(TARGET_SERVER).sh
	chmod +x $(BIN_PATH)$(TARGET_SERVER).sh	

clean: clean_linux
	$(warning Cleaned!)

clean_linux:
	rm -rf $(BIN_PATH)$(OBJ_PATH)

info:
	$(info $(shell ls $(BIN_PATH)$(OBJ_PATH)))

################################################################################
# Windows 
################################################################################
all_windows: LDFLAGS += -L./lib/win -lIrrlicht -lopengl32 -lm -lfmod -lfmodstudio -lRakNetDLL -lmingw32 -lSDL2 -lglew32 -lassimp -lsfml-graphics-2 -lsfml-system-2 -lRedPanda
all_windows:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)

################################################################################
# MAC
################################################################################
all_mac: LDFLAGS += -L./lib/mac -lfmod -lfmodstudio -lRakNetDLL -lIrrlicht -lSDL2 -lGLEW -lassimp -lsfml-graphics -lsfml-system -lRedPanda
all_mac: LDFLAGS += -dylib_file \
/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib:\
/System/Library/Frameworks/OpenGL.framework/Versions/A/Libraries/libGL.dylib \
-framework Cocoa -framework OpenGL -framework Carbon -framework IOKit


sh_valueMAC := "export LD_LIBRARY_PATH=lib/mac/ && bin/XKating"

all_mac:
	$(CXX) $(CXXFLAGS) $(addprefix $(BIN_PATH)$(OBJ_PATH),$(shell ls $(BIN_PATH)$(OBJ_PATH))) -o $(BIN_PATH)$(TARGET) $(LDFLAGS)
	rm -rf $(BIN_PATH)$(TARGET).sh
	@echo $(sh_valueMAC) >> $(BIN_PATH)$(TARGET).sh
	chmod +x $(BIN_PATH)$(TARGET).sh	
	
install_mac:
	brew reinstall sdl2 
	brew reinstall sfml 
	brew reinstall assimp 
	brew reinstall glew

.PHONY: all clean clean_linux info install win
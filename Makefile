CXX      := g++
CXXFLAGS := -std=c++17 -Wall -O2 -DNDEBUG -DSFML_STATIC -DCURL_STATICLIB -DWIN32 -static -pipe

INCLUDES := -I"SFML-2.6.2/include" \
            -I"CURL"               \
            -I"CURL/include"       \
            -Iinclude

LIB_DIRS := -L"SFML-2.6.2/lib" \
            -L"CURL/lib"

LIBS := -lsfml-audio-s    \
        -lsfml-graphics-s \
        -lsfml-window-s   \
        -lsfml-network-s  \
        -lsfml-system-s   \
        -lsfml-main       \
        -lopenal32        \
        -lflac            \
        -lvorbisenc       \
        -lvorbisfile      \
        -lvorbis          \
        -logg             \
        -lopengl32        \
        -lfreetype        \
        -lwinmm           \
        -lgdi32           \
        -lws2_32          \
        -lbcrypt          \
        -lcurl            \
        -static-libgcc    \
        -static-libstdc++

TARGET  := LaserTank.exe
OBJ_DIR := obj

SRC := $(wildcard src/*.cpp)              \
       $(wildcard src/game/*.cpp)         \
       $(wildcard src/network/*.cpp)      \
       $(wildcard src/utils/*.cpp)        \
       $(wildcard src/application/*.cpp)  \
       $(wildcard include/mainMenu/*.cpp) \
       tinyxml2/tinyxml2.cpp

OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

.PHONY: all clean run dirs

all: dirs $(TARGET)

dirs:
	@if not exist "$(OBJ_DIR)"                      mkdir "$(OBJ_DIR)"
	@if not exist "$(OBJ_DIR)\src"                  mkdir "$(OBJ_DIR)\src"
	@if not exist "$(OBJ_DIR)\src\game"             mkdir "$(OBJ_DIR)\src\game"
	@if not exist "$(OBJ_DIR)\src\network"          mkdir "$(OBJ_DIR)\src\network"
	@if not exist "$(OBJ_DIR)\src\utils"            mkdir "$(OBJ_DIR)\src\utils"
	@if not exist "$(OBJ_DIR)\src\application"      mkdir "$(OBJ_DIR)\src\application"
	@if not exist "$(OBJ_DIR)\tinyxml2"             mkdir "$(OBJ_DIR)\tinyxml2"
	@if not exist "$(OBJ_DIR)\include\mainMenu"     mkdir "$(OBJ_DIR)\include\mainMenu"

$(TARGET): $(OBJ)
	@echo [LINK] Linking $(TARGET)
	$(CXX) $(OBJ) -o $(TARGET) $(LIB_DIRS) $(LIBS)

$(OBJ_DIR)/%.o: %.cpp
	@echo [C++] Compiling $
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo Cleaning...
	-rmdir /S /Q "$(OBJ_DIR)" 2>nul || exit 0
	-del /Q "$(TARGET)"       2>nul || exit 0

run: all
	@echo Running...
	./$(TARGET)
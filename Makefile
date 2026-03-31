CXX := g++
CXXFLAGS := -std=c++17 -Wall -O2 -DNDEBUG -DSFML_STATIC -DCURL_STATICLIB -DWIN32 -static -pipe

INCLUDES := -I"SFML-2.6.2/include" \
            -I"CURL" \
            -Iinclude

LIB_DIRS := -L"SFML-2.6.2/lib" \
            -L"CURL/lib"

LIBS := -lsfml-audio-s \
        -lsfml-graphics-s \
        -lsfml-window-s \
        -lsfml-network-s \
        -lsfml-system-s \
        -lsfml-main \
        -lopenal32 \
        -lflac \
        -lvorbisenc \
        -lvorbisfile \
        -lvorbis \
        -logg \
        -lopengl32 \
        -lfreetype \
        -lwinmm \
        -lgdi32 \
        -lws2_32 \
        -lbcrypt \
        -lcurl \
        -static-libgcc \
        -static-libstdc++

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.cpp) \
	tinyxml2/tinyxml2.cpp

OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

TARGET := LaserTank.exe

all: $(TARGET)

dirs:
	@if not exist "$(OBJ_DIR)" (mkdir "$(OBJ_DIR)")
	@if not exist "$(BIN_DIR)" (mkdir "$(BIN_DIR)")

$(TARGET): $(OBJ)
	@echo [LINK] Linking $(TARGET)
	$(CXX) $(OBJ) -o $(TARGET) $(LIB_DIRS) $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo [C++] Compiling $<
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


clean:
	@echo Cleaning...
	-del /Q $(OBJ_DIR)\*.o 2>nul || exit 0
	-del /Q $(TARGET) 2>nul || exit 0

run: all
	@echo Running...
	$(TARGET)

.PHONY: all clean run dirs
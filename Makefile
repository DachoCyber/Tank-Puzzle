CXX ?= g++

SFML_VER := 2.6.2
SFML_DIR := SFML-$(SFML_VER)
CURL_DIR := CURL

ifeq ($(OS),Windows_NT)
    PLATFORM := windows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        PLATFORM := macos
    else
        PLATFORM := linux
    endif
endif

CXX_ID := $(shell $(CXX) --version 2>&1)
ifneq (,$(findstring clang,$(CXX_ID)))
    COMPILER := clang
else
    COMPILER := gcc
endif

TRIPLE := $(shell $(CXX) -dumpmachine)
ifneq (,$(findstring x86_64,$(TRIPLE)))
    ARCH := 64
else ifneq (,$(findstring aarch64,$(TRIPLE)))
    ARCH := arm64
else ifneq (,$(findstring arm64,$(TRIPLE)))
    ARCH := arm64
else
    ARCH := 32
endif

# Pick the matching prebuilt SFML 2.6.2 package for this system.
ifeq ($(PLATFORM),windows)
    ifeq ($(ARCH),64)
        SFML_ARCHIVE := SFML-$(SFML_VER)-windows-gcc-13.1.0-mingw-64-bit.zip
    else
        SFML_ARCHIVE := SFML-$(SFML_VER)-windows-gcc-13.1.0-mingw-32-bit.zip
    endif
else ifeq ($(PLATFORM),macos)
    ifeq ($(ARCH),arm64)
        SFML_ARCHIVE := SFML-$(SFML_VER)-macOS-clang-arm64.tar.gz
    else
        SFML_ARCHIVE := SFML-$(SFML_VER)-macOS-clang-64-bit.tar.gz
    endif
else
    SFML_ARCHIVE := SFML-$(SFML_VER)-linux-gcc-64-bit.tar.gz
endif

SFML_URL := https://www.sfml-dev.org/files/$(SFML_ARCHIVE)

# Prebuilt libcurl for MinGW is no longer published upstream, so we host a slim
# build as a release asset on the project repo and pull the matching one.
CURL_BASE := https://github.com/DachoCyber/LaserTankVSCode/releases/download/deps
ifeq ($(ARCH),64)
    CURL_ARCHIVE := curl-win64-mingw.zip
else
    CURL_ARCHIVE := curl-win32-mingw.zip
endif
CURL_URL := $(CURL_BASE)/$(CURL_ARCHIVE)

CXXFLAGS := -std=c++17 -Wall -O2 -DNDEBUG -pipe

ifeq ($(PLATFORM),windows)
    CXXFLAGS += -DSFML_STATIC -DWIN32 -static
    INCLUDES := -I"$(SFML_DIR)/include" -I"$(CURL_DIR)" -I"$(CURL_DIR)/include" -Iinclude
    LIB_DIRS := -L"$(SFML_DIR)/lib" -L"$(CURL_DIR)/lib"
    LIBS := -lsfml-audio-s -lsfml-graphics-s -lsfml-window-s -lsfml-network-s -lsfml-system-s -lsfml-main \
            -lopenal32 -lflac -lvorbisenc -lvorbisfile -lvorbis -logg -lopengl32 -lfreetype \
            -lwinmm -lgdi32 -lws2_32 -lbcrypt -lcurl \
            -static-libgcc -static-libstdc++
    TARGET := LaserTank.exe
else
    INCLUDES := -I$(SFML_DIR)/include -Iinclude
    LIB_DIRS := -L$(SFML_DIR)/lib -Wl,-rpath,$(SFML_DIR)/lib
    LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network -lcurl
    TARGET := LaserTank
endif

OBJ_DIR := obj
BUILD_LOG := build.log

SRC := $(wildcard src/*.cpp)              \
       $(wildcard src/game/*.cpp)         \
       $(wildcard src/network/*.cpp)      \
       $(wildcard src/utils/*.cpp)        \
       $(wildcard src/application/*.cpp)  \
       $(wildcard include/mainMenu/*.cpp) \
       tinyxml2/tinyxml2.cpp

OBJ := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

.PHONY: all clean run deps dirs logstart

all: deps dirs logstart $(TARGET)

ifeq ($(PLATFORM),windows)

logstart:
	@echo Build log %DATE% %TIME% > "$(BUILD_LOG)"

deps:
	@if not exist "$(SFML_DIR)\include" echo Downloading $(SFML_ARCHIVE)
	@if not exist "$(SFML_DIR)\include" curl -L -o sfml.zip "$(SFML_URL)"
	@if not exist "$(SFML_DIR)\include" tar -xf sfml.zip
	@if exist sfml.zip del /Q sfml.zip
	@if not exist "$(CURL_DIR)\lib" echo Downloading libcurl
	@if not exist "$(CURL_DIR)" mkdir "$(CURL_DIR)"
	@if not exist "$(CURL_DIR)\lib" curl -L -o curlpkg.zip "$(CURL_URL)"
	@if not exist "$(CURL_DIR)\lib" tar -xf curlpkg.zip -C "$(CURL_DIR)"
	@if exist curlpkg.zip del /Q curlpkg.zip
	@if not exist openal32.dll copy "$(SFML_DIR)\bin\openal32.dll" . >nul
	@if exist "$(CURL_DIR)\bin" copy /Y "$(CURL_DIR)\bin\*.dll" . >nul

dirs:
	@if not exist "$(OBJ_DIR)"                  mkdir "$(OBJ_DIR)"
	@if not exist "$(OBJ_DIR)\src"              mkdir "$(OBJ_DIR)\src"
	@if not exist "$(OBJ_DIR)\src\game"         mkdir "$(OBJ_DIR)\src\game"
	@if not exist "$(OBJ_DIR)\src\network"      mkdir "$(OBJ_DIR)\src\network"
	@if not exist "$(OBJ_DIR)\src\utils"        mkdir "$(OBJ_DIR)\src\utils"
	@if not exist "$(OBJ_DIR)\src\application"  mkdir "$(OBJ_DIR)\src\application"
	@if not exist "$(OBJ_DIR)\tinyxml2"         mkdir "$(OBJ_DIR)\tinyxml2"
	@if not exist "$(OBJ_DIR)\include\mainMenu" mkdir "$(OBJ_DIR)\include\mainMenu"

clean:
	@echo Cleaning...
	-rmdir /S /Q "$(OBJ_DIR)" 2>nul || exit 0
	-del /Q "$(TARGET)"       2>nul || exit 0
	-del /Q "$(BUILD_LOG)"    2>nul || exit 0

else

logstart:
	@echo "Build log $$(date)" > "$(BUILD_LOG)"

deps:
	@if [ ! -d "$(SFML_DIR)" ]; then \
	  echo "Downloading $(SFML_ARCHIVE)"; \
	  curl -L -o sfml.tar.gz "$(SFML_URL)"; \
	  tar -xf sfml.tar.gz; \
	  rm -f sfml.tar.gz; \
	fi

dirs:
	@mkdir -p $(OBJ_DIR)/src/game $(OBJ_DIR)/src/network $(OBJ_DIR)/src/utils \
	         $(OBJ_DIR)/src/application $(OBJ_DIR)/tinyxml2 $(OBJ_DIR)/include/mainMenu

clean:
	@echo Cleaning...
	-rm -rf $(OBJ_DIR)
	-rm -f $(TARGET)
	-rm -f $(BUILD_LOG)

endif

$(TARGET): $(OBJ)
	@echo [LINK] Linking $(TARGET)
	@echo ===== Linking $(TARGET) ===== >> "$(BUILD_LOG)"
	@$(CXX) $(OBJ) -o $(TARGET) $(LIB_DIRS) $(LIBS) >> "$(BUILD_LOG)" 2>&1

$(OBJ_DIR)/%.o: %.cpp
	@echo [C++] Compiling $<
	@echo ===== Compiling $< ===== >> "$(BUILD_LOG)"
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@ >> "$(BUILD_LOG)" 2>&1

run: all
	@echo Running...
	./$(TARGET)

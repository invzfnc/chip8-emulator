CXX	     := C:/msys64/ucrt64/bin/g++.exe
CXXFLAGS := -std=c++17 -Wall -Wextra

SDL_INCLUDES := -IC:/msys64/ucrt64/include
SDL_LIBS:=      -LC:/msys64/ucrt64/lib -lSDL3

TARGET   := chip8.exe
SOURCES  := main.cpp chip8.cpp
HEADERS  := chip8.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SDL_INCLUDES) $(SOURCES) -o $(TARGET) $(SDL_LIBS)

clean:
	#rm -f $(TARGET) $(TARGET).exe
	del /Q $(TARGET).exe 2>NUL


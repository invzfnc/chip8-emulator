CXXFLAGS := -std=c++17 -Wall -Wextra
TARGET   := chip8
SOURCES  := main.cpp chip8.cpp
HEADERS  := chip8.h

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

clean:
	#rm -f $(TARGET) $(TARGET).exe
	del /Q $(TARGET).exe 2>NUL


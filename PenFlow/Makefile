# Compiler
CXX = g++

# Compiler Flags
CXXFLAGS = -std=c++11 -I C:/external_libs_files/fltk-1.4.0-1

# Linker Flags
LDFLAGS = -L C:/external_libs/fltk/lib -lfltk -lfltk_images -lcomctl32 -lole32 -luuid -lgdi32 -lws2_32 -lgdiplus -lwinspool -lcomdlg32
# Target
TARGET = Paint.exe

# Source Files
SRCS = Paint.cpp

# Build Rules
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LDFLAGS)

clean:
	del $(TARGET)
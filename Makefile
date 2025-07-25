CXX = em++

# Source files
SRC = main.cpp game.cpp

# Include directories
INCLUDES = -I./src \
           -I/home/aj/Documents/git/misc/raylib/web/src \

# Library directories
LIBDIRS = -L/home/aj/Documents/git/misc/raylib/web/src

# Libraries
LIBS = -lraylib

# Emscripten flags
EMFLAGS = -s USE_GLFW=3 \
          -s ASYNCIFY \
          -s ALLOW_MEMORY_GROWTH \
          -s ASSERTIONS \
          -s SINGLE_FILE \
          -O2 \
          -D PLATFORM_WEB=1 \
          --embed-file resources

# Output file
OUTPUT = ./builds/web/tamamatch3.html

# Object files
OBJ = $(SRC:.cpp=.o)

# Default target
all: $(OUTPUT)

# Linking
$(OUTPUT): $(OBJ)
	$(CXX) $(OBJ) $(INCLUDES) $(LIBDIRS) $(LIBS) $(EMFLAGS) -o $@

# Compilation
%.o: %.cpp
	$(CXX) $(INCLUDES) $(EMFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(OBJ) $(OUTPUT)
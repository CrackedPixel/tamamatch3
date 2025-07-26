CXX = g++
STRIP = strip
PACK = upx --lzma -9

DEFINES = -DPLATFORM_DESKTOP

# Source files
SRC = \
    src/audiomanager.cpp \
    src/deviceinfo.cpp \
    src/game.cpp \
    src/main.cpp \
    src/scene_splash.cpp \
    src/scene_tama.cpp \
    src/scenemanager.cpp \
    src/tamapetai.cpp \
    src/tamaui.cpp

# Include directories ONLY (do NOT list .hpp files here)
INCLUDES = -I./src \
	-I/home/aj/Documents/raylib/latest/release/include \
	-I/home/aj/Documents/git/misc/emsdk/upstream/emscripten/system/include

LIBDIRS = -L/home/aj/Documents/raylib/latest/release/lib

LIBS = -lraylib

OUTFILE = tamamatch3.exe
OUTPUT = ./builds/desktop/$(OUTFILE)
PACKAGEPATH = ./builds/desktop/package
RESOURCES = ./resources

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -std=c++11 -Wall -Wextra $(DEFINES) $(INCLUDES)

LDFLAGS = $(LIBDIRS) $(LIBS)

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CXX) $(OBJ) $(LDFLAGS) -o $@
	mkdir -p $(PACKAGEPATH)/resources
	mkdir -p ./builds/desktop/
	cp -r $(RESOURCES) ./builds/desktop/
	cp -r $(RESOURCES)/ $(PACKAGEPATH)/
	cp ./builds/desktop/$(OUTFILE) $(PACKAGEPATH)/
	$(STRIP) $(PACKAGEPATH)/$(OUTFILE)
	$(PACK) $(PACKAGEPATH)/$(OUTFILE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(OUTPUT)
	rm -rf $(PACKAGEPATH)

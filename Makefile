CXX = /usr/bin/x86_64-w64-mingw32-g++
STRIP = strip
PACK = echo upx --lzma -9

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
	-I./external \
	-I/home/aj/Documents/raylib/win/release/include

LIBDIRS = -L/home/aj/Documents/raylib/win/release/lib

LIBS = -lraylib -lgdi32 -lwinmm

OUTFILE = tamamatch3.exe
OUTPUT = ./builds/desktop/$(OUTFILE)
PACKAGEPATH = ./builds/desktop/package
RESOURCES = ./resources

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -static -static-libgcc -static-libstdc++ -std=c++11 -Wall -Wextra -Oz -flto $(DEFINES) $(INCLUDES)

LDFLAGS = -static -static-libgcc -static-libstdc++ -flto $(LIBDIRS) $(LIBS)

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

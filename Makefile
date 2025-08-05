CXX = /usr/bin/x86_64-w64-mingw32-g++
STRIP = strip
PACK = upx --lzma -9

DEFINES = -DPLATFORM_DESKTOP -DPLATFORM_WINDOWS

# Source files
SRC = \
    src/audiomanager.cpp \
    src/deviceinfo.cpp \
    src/game.cpp \
    src/main.cpp \
    src/scene_collection.cpp \
    src/scene_credits.cpp \
    src/scene_mastermind.cpp \
    src/scene_menu.cpp \
    src/scene_shop.cpp \
    src/scene_splash.cpp \
    src/scene_tama.cpp \
    src/scenemanager.cpp \
    src/tamapetai.cpp \
    src/tamaui.cpp

# Include directories ONLY (do NOT list .hpp files here)
INCLUDES = -I./src \
	-I./external \
	-I./external/raylib_win/

LIBDIRS = -L./external/raylib_win/

LIBS = -lraylib -lgdi32 -lwinmm

OUTFILE = tamagacha.exe
OUTPUT = ./builds/desktop/$(OUTFILE)
PACKAGEPATH = ./builds/desktop/package
RESOURCES = ./resources

OBJ = $(SRC:.cpp=.o)

CXXFLAGS = -static -static-libgcc -static-libstdc++ -mwindows -std=c++17 -Wall -Wextra -Oz $(DEFINES) $(INCLUDES)

LDFLAGS = -static -static-libgcc -static-libstdc++ -mwindows $(LIBDIRS) $(LIBS)

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CXX) $(OBJ) icon.o $(LDFLAGS) -o $@
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

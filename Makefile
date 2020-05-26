# Nathaniel Collier
# 8/25/2007
# SDL Test Makefile


#Example:   export set NAC_LIB_DIR=~/binCLib
ifeq ($(NAC_LIB_DIR),)
  $(error NAC_LIB_DIR is not set)
endif


CC = g++

EXE_FILE = crazy8.exe

C_FLAGS =  -g3 -O0

# -lws2_32  : is the winsock 2 library for windows include this if you included <Winsock2.h>
L_FLAGS =   `sdl-config --cflags --libs`  -lSDL_ttf -lSDL_net -lws2_32 -lsdlgfx


C_FILES = crazy8.cpp

H_FILES = 

LIB_DIR = $(NAC_LIB_DIR)
LIB_FILES = 
L_FILES := $(patsubst %, $(LIB_DIR)/lib%.a, $(LIB_FILES))

O_FILES := $(patsubst %.cpp,o/%.o,$(C_FILES))

all: 
	mkdir -p o
	make -s $(EXE_FILE)

$(EXE_FILE): $(O_FILES)
	$(CC) -o $(EXE_FILE) $(O_FILES) $(L_FILES)  $(L_FLAGS)
	echo "Done compiling $(EXE_FILE).";
	chmod 777 $(EXE_FILE)
	chmod 777 $(O_FILES)

clean:
	rm -f $(O_FILES) $(EXE_FILE)

o/%.o: %.cpp
	echo "  Compiling $@....";
	$(CC) -c $(C_FLAGS) $< -o $@

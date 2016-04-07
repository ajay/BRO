CC 			= gcc
CFLAGS 		= -pedantic \
				-Wall \
				-Werror \
				-std=gnu99 \
				-O3 \
				-g \
				-o

CPPC 		= g++
CPPFLAGS 	= `pkg-config --cflags sdl2` \
				-pedantic \
				-Wall \
				-Werror \
				-Wno-unused-variable \
				-std=c++11 \
				-O3 \
				-g \
				-o

LIBS 		= `pkg-config --libs sdl2` \
 				-pthread \
 				-lSDL2_ttf

COMPILEC 	= $(CC) $(CFLAGS)
COMPILECPP 	= $(CPPC) $(CPPFLAGS)

OBJECTS 	= serial.o window.o BruhBot.o planner.o slam.o

all: $(OBJECTS) bruh

bruh: $(OBJECTS)
	$(COMPILECPP) $@ $^ $(LIBS)

%.o: %.c
	$(COMPILEC) $@ -c $<

%.o: %.cpp
	$(COMPILECPP) $@ -c $<

clean:
	rm -rfv *.o bruh

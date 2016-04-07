CPPC 		= g++
CPPFLAGS 	= -pedantic \
				-Wall \
				-Werror \
				-Wno-unused-variable \
				-std=c++11 \
				-O3 \
				-g \
				-o

COMPILECPP 	= $(CPPC) $(CPPFLAGS)

OBJECTS 	= cell.o maze.o mazeMain.o

all: $(OBJECTS) maze

maze: $(OBJECTS)
	$(COMPILECPP) $@ $^ $(LIBS)

%.o: %.cpp
	$(COMPILECPP) $@ -c $<

clean:
	rm -rfv *.o maze
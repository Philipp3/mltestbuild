CC=g++
CFLAGS=-c -Wall -Werror -std=c++0x -lmatio -O2
LDFLAGS=-lmatio
SOURCES=test.cpp Matfile.cpp molecule.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

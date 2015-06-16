CC=g++
CFLAGS=-c -Wall -Werror -std=c++0x -O2
LDFLAGS=
LIB= -lmatio -lgsl -lgslcblas -lm
SOURCES=test.cpp Matfile.cpp molecule.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=test

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(LIB) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

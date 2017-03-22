CC=g++
CFLAGS=-w -c -std=c++11
LDFLAGS=-L/usr/include -L/opt/local/lib -L/usr/local/lib

SOURCES=$(wildcard *.cpp wildcard Accounts/*.cpp wildcard Areas/*.cpp wildcard Characters/*.cpp wildcard Classes/*.cpp wildcard Config/*.cpp wildcard Planets/*.cpp wildcard Races/*.cpp wildcard Ships/*.cpp wildcard Skexml/*.cpp wildcard Skills/*.cpp wildcard States/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=SkelMUD

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ -pthread

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJECTS)

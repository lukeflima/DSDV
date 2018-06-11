EXECUTABLE = out

CXX = g++

CXXFLAGS = -std=c++11  

CPPFLAGS += -MMD \
			-I/usr/include

LDFLAGS = -L/usr/lib/gcc/x86_64-linux-gnu/4.8 \
		  -L/usr/lib/x86_64-linux-gnu \
		  -L/usr/lib \
	      -L/usr/lib

LDLIBS = -lpthread 

SOURCES = $(wildcard *.cpp)

OBJECTS = $(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)

.cpp.o:

-include $(OBJECTS:.o=.d)

.PHONY: clean distclean

clean:
	rm *.o *.d

clear: clean
	rm $(EXECUTABLE)

execute: out

	./out <input.txt> log.txt

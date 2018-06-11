TARGET_EXEC ?= a.out

SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CPPFLAGS ?= $(INC_FLAGS) -std=c++11

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $^ -o $@ $(LDLIBS)

.cpp.o:

-include $(OBJECTS:.o=.d)


.PHONY: clean #tests	

clean:
	rm *.o *.d

execute: out

	./a.out < input.txt > log.txt


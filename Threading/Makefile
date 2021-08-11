.PHONEY: all

SOURCE=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCE))
BIN=thread

CXXFLAGS+=-O3

LDLIBS+=-lpthread
LDFLAGS+=-O3


all: $(BIN)
	echo $(SOURCE)
	echo $(OBJ)

run: $(BIN)
	./$(BIN)

$(BIN): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS) $(LDFLAGS)

clean:
	-rm $(OBJS) $(BIN)

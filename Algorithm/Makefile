.PHONEY: all

SOURCE=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCE))
BIN=algorithm

CXXFLAGS+=-O3 -std=c++17

LDLIBS+=-lpthread
LDFLAGS+=-O3


all: $(BIN)
	echo $(SOURCE)
	echo $(OBJ)

run: $(BIN)
	-./$(BIN)

$(BIN): $(OBJS)
	$(CXX) $^ -o $@ $(LDLIBS) $(LDFLAGS)

clean:
	-rm $(OBJS) $(BIN)

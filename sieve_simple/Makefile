.PHONEY: all

SOURCE=$(wildcard *.cpp)
OBJS=$(patsubst %.cpp,%.o,$(SOURCE))
BIN=sieve_simple



all: $(BIN)
	echo $(SOURCE)
	echo $(OBJ)

run: $(BIN)
	./$(BIN)

$(BIN): $(OBJS)
	$(CXX) $^ -o $@

clean:

HEADER := compiler.h block.h blockchain.h sha256.h vm.h 
SRC    := compiler.cpp block.cpp blockchain.cpp sha256.cpp vm.cpp 
TARGET := main test

all: $(TARGET)

main: main.cpp $(HEADER) $(SRC) 
	g++ -std=c++11 -o $@ $< $(SRC)

test: test.cpp $(HEADER) $(SRC) 
	g++ -std=c++11 -o $@ $< $(SRC)

clean:
	rm $(TARGET)

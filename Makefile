CC     = g++
CFLAGS = -lm -std=c++0x -ggdb -O2
OBJ    = $(addsuffix .o, $(basename $(shell find . -name "*.cpp")))
BIN    = metnum

.PHONY: all informe clean

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o metnum

%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

informe:
	make -C tex

clean:
	rm -f $(BIN) $(shell find . -name "*.o")
	make -C tex clean
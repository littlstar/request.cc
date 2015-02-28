
CFLAGS = -std=c99
CXXFLAGS = -Ideps
LDFLAGS = -lcurl
VALGRIND_OPTS ?= --leak-check=full --error-exitcode=1
SRC_CC = request.o $(wildcard deps/*/*.cc)
SRC_C = $(wildcard deps/*/*.c)
OBJS = $(SRC_CC:.cc=.o) $(SRC_C:.c=.o)

default: test
	./$<

test: test.o $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

valgrind: test
	valgrind $(VALGRIND_OPTS) ./$<

clean:
	rm -f test.o test
	rm -f $(OBJS)

.PHONY: default valgrind clean

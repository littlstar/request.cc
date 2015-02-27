
CXXFLAGS = -Ideps
LDFLAGS = -lcurl
VALGRIND_OPTS ?= --leak-check=full --error-exitcode=1

default: test
	./$<

test: test.o request.o deps/trim/trim.o
	$(CXX) $^ -o $@ $(LDFLAGS)

valgrind: test
	valgrind $(VALGRIND_OPTS) ./$<

clean:
	rm -f test.o request.o deps/trim/trim.o
	rm -f test

.PHONY: default valgrind clean

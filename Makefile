CXX = clang++

CXXFLAGS = -g -O2 -Wno-deprecated-register -std=c++11

q: build/q.tab.c build/lex.yy.c frontend/AST.cpp runtime/Visitor.cpp runtime/Value.hpp runtime/Utils.cpp runtime/Globals.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -I. main.cpp runtime/Globals.cpp runtime/Visitor.cpp frontend/AST.cpp -x c++ build/q.tab.c -x c++ build/lex.yy.c -lfl -o build/q

build/q.tab.c: frontend/q.y
	mkdir -p build
	bison frontend/q.y --defines=build/q.tab.h -o build/q.tab.c

build/lex.yy.c: frontend/q.l
	mkdir -p build
	flex -o build/lex.yy.c frontend/q.l

.PHONY: clean
clean:
	rm -f build/*

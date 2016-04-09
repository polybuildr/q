q: q.tab.c lex.yy.c AST.cpp AST.hpp Visitor.cpp Visitor.hpp Value.hpp Utils.cpp ObjectPool.hpp Globals.hpp Globals.cpp
	clang++ -g -O2 -Wno-deprecated-register -std=c++11 Globals.cpp main.cpp Visitor.cpp AST.cpp -x c++ q.tab.c -x c++ lex.yy.c -lfl -o q

q.tab.c: q.y
	bison -d q.y

lex.yy.c: q.l
	flex -o lex.yy.c q.l

.PHONY: clean
clean:
	rm -f lex.yy.c q.tab.c q.tab.h

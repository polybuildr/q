q: q.tab.c lex.yy.c AST.cpp Visitor.cpp
	clang++ -Wno-deprecated-register -std=c++11 Visitor.cpp AST.cpp q.tab.c lex.yy.c -lfl -o q

q.tab.c: q.y
	bison -d q.y

lex.yy.c: q.l
	flex -o lex.yy.c q.l

clean:
	rm -f lex.yy.c q.tab.c q.tab.h

CXX = g++-8
EXE = iplC
CXXDEBUG = -g -Wall
CXXSTD = -std=c++11
SCANNER = scanner.l


.PHONY: all
all : parser.o scanner.o type.o ast.o  symbtab.o main.o
	$(CXX) $(CXXDEBUG) -o $(EXE) main.o parser.o scanner.o ast.o type.o symbtab.o 	

main.o : main.cpp
	$(CXX) $(CXXDEBUG) -c main.cpp -o main.o

parser.o : parser.yy scanner.l scanner.hh
	bison -d -v $<
	$(CXX) $(CXXDEBUG) -c parser.tab.cc -o parser.o

scanner.o : $(SCANNER) scanner.hh parser.tab.hh parser.tab.cc
	flex++ --outfile=scanner.yy.cc  $<
	$(CXX)  $(CXXDEBUG) -c scanner.yy.cc -o scanner.o

ast.o : ast.cpp ast.hh
	$(CXX) $(CXXDEBUG) -c ast.cpp -o ast.o

type.o : type.cpp type.hh ast.o
	$(CXX) $(CXXDEBUG) -c type.cpp -o type.o

symbtab.o : symbtab.cpp symbtab.hh type.hh type.cpp
	$(CXX) $(CXXDEBUG) -c symbtab.cpp -o symbtab.o

.PHONY: clean
clean:
	rm -f *.o *.tab.* *.yy.* *.output $(EXE) location.hh position.hh stack.hh



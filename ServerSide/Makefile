CXXFlags = -std=c++11 -O2

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CXX = clang++
else
	CXX = g++
endif

all: main

Board.o: Board.cpp Board.h
	$(CXX) $(CXXFlags) -c Board.cpp

AI.o: AI.cpp AI.h Board.h MinMaxTree2.h
	$(CXX) $(CXXFlags) -c AI.cpp

Evaluator.o: Evaluator.cpp Evaluator.h TokenStream.h Game.h SMPI.h
	$(CXX) $(CXXFlags) -c Evaluator.cpp

SMPI.o: SMPI.cpp SMPI.h Game.h TokenStream.h NetSocket.h
	$(CXX) $(CXXFlags) -c SMPI.cpp

Game.o: Game.cpp Game.h Board.h AI.h MinMaxTree2.h TokenStream.h Evaluator.h AI.h
	$(CXX) $(CXXFlags) -c Game.cpp

NetSocket.o: NetSocket.cpp NetSocket.h
	$(CXX) $(CXXFlags) -lpthread -c NetSocket.cpp

Server.o: Server.cpp NetSocket.h
	$(CXX) $(CXXFlags) -lpthread -c Server.cpp

TokenStream.o: TokenStream.cpp TokenStream.h
	$(CXX) $(CXXFlags) -c TokenStream.cpp

main: Board.o Evaluator.o Game.o NetSocket.o Server.o TokenStream.o AI.o MinMaxTree2.o SMPI.o
	$(CXX) $(CXXFlags) -lpthread Board.o Evaluator.o Game.o NetSocket.o Server.o TokenStream.o AI.o MinMaxTree2.o SMPI.o -o main

MinMaxTree2.o: MinMaxTree2.cpp MinMaxTree2.h Board.h
	$(CXX) $(CXXFlags) -c MinMaxTree2.cpp Board.cpp

test: Board.o Evaluator.o Game.o TokenStream.o testmain.o
	$(CXX) $(CXXFlags) Board.o Evaluator.o Game.o TokenStream.o testmain.o -o test

clean:
	rm *.o main

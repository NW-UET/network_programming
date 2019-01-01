server: server.o ListUpdate.o Confirm.o convert.o
	g++ -o server -pthread server.cpp ListUpdate.o Confirm.o
server.o: server.cpp package.h convert.h
	g++ -pthread -c server.cpp
ListUpdate.o: ListUpdate.cpp package.h
	g++ -c ListUpdate.cpp
Confirm.o: Confirm.cpp package.h
	g++ -c Confirm.cpp
convert.o: convert.cpp convert.h
	g++ -c convert.cpp

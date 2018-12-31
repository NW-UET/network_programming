CXX = g++
CXXFLAGS  = -std=c++0x -Wall

SRC = server.cpp client.cpp FileListUpdateRequest.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = server client FileListUpdateRequest

all: server client

server: FileListUpdateRequest.o server.o
	$(CXX) $(CXXFLAGS) FileListUpdateRequest.o server.o -o server

client: FileListUpdateRequest.o client.o
	$(CXX) $(CXXFLAGS) FileListUpdateRequest.o client.o -o client

FileListUpdateRequest.o: FileListUpdateRequest.cpp
	$(CXX) $(CXXFLAGS) -c FileListUpdateRequest.cpp -o FileListUpdateRequest.o

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o client.o

clean:
	$(RM) $(TARGET) $(OBJ)

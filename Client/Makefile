CXX = g++
CXXFLAGS  = -std=c++0x -Wall

SRC = server.cpp client.cpp FileListUpdateRequest.cpp ListHostsRequest.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = server client

all: server client

server: FileListUpdateRequest.o ListHostsRequest.o server.o utils.o
	$(CXX) $(CXXFLAGS) FileListUpdateRequest.o ListHostsRequest.o server.o utils.o -o server

client: FileListUpdateRequest.o ListHostsRequest.o client.o utils.o
	$(CXX) $(CXXFLAGS) FileListUpdateRequest.o ListHostsRequest.o client.o utils.o -o client

FileListUpdateRequest.o: FileListUpdateRequest.cpp
	$(CXX) $(CXXFLAGS) -c FileListUpdateRequest.cpp -o FileListUpdateRequest.o

ListHostsRequest.o: ListHostsRequest.cpp
	$(CXX) $(CXXFLAGS) -c ListHostsRequest.cpp -o ListHostsRequest.o

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o client.o

utils.o: utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

clean:
	$(RM) $(TARGET) $(OBJ)

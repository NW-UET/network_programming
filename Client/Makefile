CXX = g++
CXXFLAGS  = -std=c++0x -Wall

MSG_SRC = FileListUpdateRequest.cpp FileListUpdateResponse.cpp ListFilesResponse.cpp ListHostsRequest.cpp ListHostsResponse.cpp
MSG_OBJ = $(MSG_SRC:.cpp=.o)
SRC = server.cpp client.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = server client

all: server client

server: msg server.o utils.o
	$(CXX) $(CXXFLAGS) $(MSG_OBJ) server.o utils.o -o server

client: msg client.o utils.o
	$(CXX) $(CXXFLAGS) $(MSG_OBJ) client.o utils.o -o client

msg: $(MSG_OBJ)
	$(foreach var, $(MSG_SRC), $(CXX) $(CXXFLAGS) -c $(var) -o $(var:.cpp=.o);)

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o client.o

utils.o: utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

clean:
	$(RM) $(TARGET) $(OBJ) $(MSG_OBJ)

CXX = g++
CXXFLAGS  = -std=c++0x -Wall

MSG_SRC = FileListUpdateRequest.cpp FileListUpdateResponse.cpp ListFilesRequest.cpp ListFilesResponse.cpp ListHostsRequest.cpp ListHostsResponse.cpp DownloadFileRequest.cpp DownloadFileResponse.cpp
MSG_OBJ = $(MSG_SRC:.cpp=.o)
SRC = server.cpp utils.cpp process.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = server

all: server msg

server: $(MSG_OBJ) server.o utils.o process.o
	$(CXX) $(CXXFLAGS) $(MSG_OBJ) server.o utils.o process.o -o server -pthread -l sqlite3

msg: $(MSG_OBJ)
	$(foreach var, $(MSG_SRC), $(CXX) $(CXXFLAGS) -c $(var) -o $(var:.cpp=.o);)

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

utils.o: utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

process.o: process.cpp
	$(CXX) $(CXXFLAGS) -c process.cpp -o process.o -l sqlite3

clean:
	$(RM) $(TARGET) $(OBJ) $(MSG_OBJ)

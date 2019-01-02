CXX = g++
CXXFLAGS  = -std=c++0x -Wall

MSG_SRC = FileListUpdateRequest.cpp FileListUpdateResponse.cpp ListFilesRequest.cpp ListFilesResponse.cpp ListHostsRequest.cpp ListHostsResponse.cpp DownloadFileRequest.cpp DownloadFileResponse.cpp
MSG_OBJ = $(MSG_SRC:.cpp=.o)
SRC = server.cpp client.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = server client

server: msg server.o utils.o
	$(CXX) $(CXXFLAGS) $(MSG_OBJ) server.o utils.o -o server -pthread

msg: $(MSG_OBJ)
	$(foreach var, $(MSG_SRC), $(CXX) $(CXXFLAGS) -c $(var) -o $(var:.cpp=.o);)

server.o: server.cpp
	$(CXX) $(CXXFLAGS) -c server.cpp -o server.o

utils.o: utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

clean:
	$(RM) $(TARGET) $(OBJ) $(MSG_OBJ)

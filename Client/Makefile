CXX = g++
CXXFLAGS  = -std=c++0x -Wall

MSG_SRC = FileListUpdateRequest.cpp FileListUpdateResponse.cpp ListFilesRequest.cpp ListFilesResponse.cpp ListHostsRequest.cpp ListHostsResponse.cpp DownloadFileRequest.cpp DownloadFileResponse.cpp
MSG_OBJ = $(MSG_SRC:.cpp=.o)
SRC = client.cpp utils.cpp
OBJ = $(SRC:.cpp=.o)

# the build target executable:
TARGET = client

all: client msg

client: $(MSG_OBJ) client.o utils.o
	$(CXX) $(CXXFLAGS) $(MSG_OBJ) client.o utils.o -o client -pthread

msg: $(MSG_OBJ)
	$(foreach var, $(MSG_SRC), $(CXX) $(CXXFLAGS) -c $(var) -o $(var:.cpp=.o);)

client.o: client.cpp
	$(CXX) $(CXXFLAGS) -c client.cpp -o client.o

utils.o: utils.cpp
	$(CXX) $(CXXFLAGS) -c utils.cpp -o utils.o

clean:
	$(RM) $(TARGET) $(OBJ) $(MSG_OBJ)

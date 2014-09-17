CXX=g++
CXXFLAGS=-std=c++11 -Isrc/ -g
LDFLAGS=-g -lev -lpthread

all: bin/xathttp

clean:
	rm -f obj/*
	rm -f bin/*

bin/xathttp: obj/request.o obj/response.o obj/server.o obj/reader.o obj/httpparser.o obj/main.o
	$(CXX) obj/request.o obj/response.o obj/server.o obj/main.o -o bin/xathttp $(LDFLAGS)

bin/test_request_response: obj/request.o obj/response.o obj/test_request_response.o
	$(CXX) obj/request.o obj/response.o obj/test_request_response.o -o bin/test_request_response $(LDFLAGS)

bin/test_server: obj/request.o obj/response.o obj/server.o obj/test_server.o
	$(CXX) obj/request.o obj/response.o obj/server.o obj/test_server.o -o bin/test_server $(LDFLAGS)

obj/request.o: src/request.cpp
	$(CXX) $(CXXFLAGS) -c src/request.cpp -o obj/request.o

obj/response.o: src/response.cpp
	$(CXX) $(CXXFLAGS) -c src/response.cpp -o obj/response.o

obj/server.o: src/server.cpp
	$(CXX) $(CXXFLAGS) -c src/server.cpp -o obj/server.o

obj/reader.o: src/reader.cpp
	$(CXX) $(CXXFLAGS) -c src/reader.cpp -o obj/reader.o

obj/httpparser.o: src/httpparser.cpp
	$(CXX) $(CXXFLAGS) -c src/httpparser.cpp -o obj/httpparser.o

obj/test_request_response.o: tests/test_request_response.cpp
	$(CXX) $(CXXFLAGS) -c tests/test_request_response.cpp -o obj/test_request_response.o

obj/test_server.o: tests/test_server.cpp
	$(CXX) $(CXXFLAGS) -c tests/test_server.cpp -o obj/test_server.o

obj/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) -c src/main.cpp -o obj/main.o

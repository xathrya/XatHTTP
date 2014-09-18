#ifndef XATHTTP_SERVER_H_
#define XATHTTP_SERVER_H_

#include <ev++.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <exception>
#include <memory>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include "httpcode.hpp"

#define BACKLOG                 20


namespace XatHTTP
{

/*
struct Callback
{
    static void sockAccept(ev::io &watcher, int revents); //struct ev_loop *loop, struct ev_io *watcher, int revents);
    static void sockRecv  (ev::io &watcher, int revents); //struct ev_loop *loop, struct ev_io *watcher, int revents);
    static void sockSend  (ev::io &watcher, int revents); //struct ev_loop *loop, struct ev_io *watcher, int revents);
};
*/

class Server
{
public:
    Server(short port);
    virtual ~Server();

    void start();

    void sockAccept(ev::io &watcher, int revents);

private:
    int         _sockfd;
    short       _port;
    bool        _running;

    ev::io      _io;
    ev::sig     _sio;

    void signalGet(ev::sig &signal, int revents);
};  /* Server Class */

class Service
{
public:
    Service(int sockfd);

private:
    ev::io      _io;
    int         _sockfd;

    std::string _message;
    int         _length = 0;
    char       *_data = nullptr;
    int         _data_type;
    int         _readStatus;

    ~Service();

    void ioAction (ev::io &watcher, int revents);
    void sockRecv (ev::io &watcher);
    void sockSend (ev::io &watcher);
};

}   /* XatHTTP Namespace */


#endif  /* XATHTTP_SERVER_H_ */
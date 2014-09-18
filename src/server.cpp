#include "response.hpp"
#include "request.hpp"
#include "server.hpp"
#include "reader.hpp"

namespace XatHTTP
{

/* Service:: */
/***********************************************************************************/
Service::Service(int sockfd)
{
    fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL, 0) | O_NONBLOCK);

    _io.set<Service, &Service::ioAction>(this);
    _io.start(_sockfd, ev::READ | ev::WRITE);
}

Service::~Service()
{
    if (_data) {
        delete _data;
    }
    _io.stop();
    close(_sockfd);
}

void Service::ioAction(ev::io &watcher, int revents)
{
    if (ev::ERROR & revents) {
        perror("invalid event");
        return;
    }

    if (EV_READ & revents) {
        sockRecv(watcher);
    }

    if (EV_WRITE & revents) {
        sockSend(watcher);
    }
}

void Service::sockRecv(ev::io &watcher)
{
    char    buffer[4096];
    ssize_t nread;
    int     fd;

    nread = recv(watcher.fd, buffer, 4096, 0);
    if (nread < 0) {
        perror("read error");
        return;
    }

    Request request = Request::parse((byte *) buffer, 4096);

    // XatHTTP::Reader reader = XatHTTP::Reader::Open(request.getFileResourcePath().data);

    std::string filename = "./www/" + request.getResourcePath();

    /*
    const char *filename = request.getFileResourcePath().data();
    */


    fd = open(filename.data(), O_RDONLY);
    if (fd < 0) {
        _readStatus = READ_STATUS_FAIL;
    } else {
        _readStatus = READ_STATUS_OK;

        _length = lseek(fd, 0, SEEK_END);
        _data   = new char[_length];
        lseek(fd, 0, SEEK_SET);
        read(fd, _data, _length);
        close(fd);
    }

    _data_type = request.getDataType();
}

void Service::sockSend(ev::io &watcher)
{
    auto sendResponse = [&](Response &response) {
        response.compileHeader();

        send(watcher.fd, response.getCompiledHeader().data(), response.getCompiledHeader().length(), 0);
        send(watcher.fd, response.getBody(), response.getLength(), 0);
    };

    if (_readStatus == READ_STATUS_OK) {
        Response response = Response::create(HTTP_200_OK, (byte *) _data, _length);

        switch(_data_type) {
        case DATA_TYPE_HTML:
            response.setContentType(CONTENT_HTML_UTF8);
            break;
        case DATA_TYPE_PLAINTEXT:
            response.setContentType(CONTENT_TEXT_PLAIN);
            break;
        }

        sendResponse(response);
    } else {
        std::string body = "<!DOCTYPE html>\n<html><head><title>404 Not Found</title></head><body><h1>404 Not Found</h1></body></html>\n";
        Response response = Response::create(HTTP_404_NOT_FOUND, (byte *) body.data(), body.length());
        response.setContentType(CONTENT_HTML_UTF8);

        sendResponse(response);
    }

    delete this;
}

/* Server:: */
/***********************************************************************************/
Server::Server(short port)
    : _port(port), _running(false)
{
    
}

Server::~Server()
{
    close(_sockfd);
}

void Server::start()
{
    // 1. Get the proper address and port
    addrinfo hints;
    addrinfo *servinfo, *p;
    char port_buffer[10];
    char ipvr_buffer[INET6_ADDRSTRLEN];
    void* addr;
    int yes = 1;

    sprintf(port_buffer, "%d", _port);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
    hints.ai_flags      = AI_PASSIVE;

    // Get my address
    if (getaddrinfo(NULL, port_buffer, &hints, &servinfo) != 0) {
        perror("getaddrinfo error");
        throw("addrinfo Error");
    }

    // Iterate and bind!
    for (p = servinfo; p != NULL; p = p->ai_next) {
        // Create socket
        if ((_sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
            perror("socket error");
            continue;
        }

        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) < 0) {
            perror("setsockopt error");
            throw "setsockopt error";
        }

        // Bind
        if (bind(_sockfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(_sockfd);
            perror("server: bind");
            continue;
        }

        fcntl(_sockfd, F_SETFL, fcntl(_sockfd, F_GETFL, 0) | O_NONBLOCK);

        if (p->ai_family == AF_INET) {
            sockaddr_in *ipv4 = (sockaddr_in *) p->ai_addr;
            addr = &(ipv4->sin_addr);
            break;
        } else {
            sockaddr_in6* ipv6 = (sockaddr_in6 *) p->ai_addr;
            addr = &(ipv6->sin6_addr);
            break;
        }

        inet_ntop(p->ai_family, addr, ipvr_buffer, sizeof(ipvr_buffer));

        break;
    }

    if (p == NULL) {
        perror("failed to acquired address");
        throw "address bind";
    }

    // Free the list
    freeaddrinfo(servinfo);

    // 2. Listen to the port
    if (listen(_sockfd, BACKLOG) < 0) {
        perror("listen error");
        throw "listen Error";
    }

    // 3. Set up the server 
    _running = true;

    printf("Server listening on address %s port %d", ipvr_buffer, _port);

    //ev_io_init(&w_accept, Callback::sockAccept, _sockfd, EV_READ);
    //ev_io_start(_loop, &w_accept);

    _io.set<Server, &Server::sockAccept>(this);
    _io.start(_sockfd, ev::READ);
    
    _sio.set<Server, &Server::signalGet>(this);
    _sio.start(SIGINT);
}

void Server::sockAccept(ev::io &watcher, int revents)
{
    if (EV_ERROR & revents) {
        perror("invalid event");
        return;
    }
    
    sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);

    int client_sockfd = accept(watcher.fd, (struct sockaddr*) &client_addr, &client_addr_size);

    if (client_sockfd < 0) {
        perror("accept error");
        return;
    }

    Service *client = new Service(client_sockfd);
}

void Server::signalGet(ev::sig &signal, int revents)
{
    signal.loop.break_loop();
}

}
#include "response.hpp"
#include "server.hpp"
#include "reader.hpp"

namespace XatHTTP
{

Server::Server(struct ev_loop* loop, short port)
    : _loop(loop), _port(port), _yes(1), _running(false)
{
    
}

Server::~Server()
{

}

void Server::acceptCallback(CB_ARGS)
{
    struct sockaddr_storage client_addr;
    socklen_t client_addr_size = sizeof(client_addr);
    int client_sockfd;

    struct ev_io *w_client = (struct ev_io*) malloc(sizeof(struct ev_io));

    client_sockfd = accept(watcher->fd, (struct sockaddr*) &client_addr, &client_addr_size);

    ev_io_init(w_client, handleCallback, client_sockfd, EV_READ);
    ev_io_start(loop, w_client);
}

void Server::handleCallback(CB_ARGS)
{
    char buffer[4096];

    memset(buffer, 0, sizeof(buffer));
    recv(watcher->fd, buffer, 4096, 0);
    printf("%s", buffer);


    std::string body = "<H1>The quick brown fox jumps over the lazy dog</H1>\n";
    XatHTTP::Response response = XatHTTP::Response::create(HTTP_200_OK, (byte*) body.data(), body.length(), false);
    response.compileHeader();

    send(watcher->fd, response.getCompiledHeader().data(), response.getCompiledHeader().length(), 0);
    send(watcher->fd, response.getBody(), response.getLength(), 0);

    ev_io_stop(loop, watcher);
    shutdown(watcher->fd, 2);
    free(watcher);
}

void Server::start()
{
    struct ev_io w_accept;

    // 1. Get the proper address and port
    addrinfo hints;
    addrinfo *servinfo, *p;
    char port_buffer[10];
    char ipvr_buffer[INET6_ADDRSTRLEN];
    void* addr;

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

        if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &_yes, sizeof(int)) < 0) {
            perror("setsockopt error");
            throw "setsockopt error";
        }

        // Bind
        if (bind(_sockfd, p->ai_addr, p->ai_addrlen) < 0) {
            close(_sockfd);
            perror("server: bind");
            continue;
        }

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

    ev_io_init(&w_accept, acceptCallback, _sockfd, EV_READ);
    ev_io_start(_loop, 0);

    while(_running) {
        ev_loop(_loop, 0);
    }
}


}
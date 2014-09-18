#include <cstring>
#include <cstdio>

#include "xathttp.hpp"
#include "server.hpp"

#define DEFAULT_PORT        13510

int main(int argc, char *argv[])
{
    int myport = DEFAULT_PORT;

    if (argc > 1) {
        myport = atoi(argv[1]);
    }

    ev::default_loop loop;

    XatHTTP::Server server(myport);

    loop.run(0);

    return 0;
}
#include <cstring>
#include <cstdio>

#include "xathttp.hpp"
#include "response.hpp"
#include "server.hpp"
#include "reader.hpp"

#define DEFAULT_PORT        13510

int main(int argc, char *argv[])
{
    int myport = DEFAULT_PORT;

    if (argc > 1) {
        myport = atoi(argv[1]);
    }

    struct ev_loop *loop = ev_default_loop(0);

    XatHTTP::Server server(loop, myport);

    ev_run(loop, 0);

    return 0;
}
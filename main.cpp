#include <iostream>
#include "tcpserver.h"
#include "log.h"

int main() {
    TCPServer server(23);
    try
    {
        server.startServer();
    }
    catch(const std::exception& e)
    {
        LOG(e.what());
    }
    
    return 0;
}

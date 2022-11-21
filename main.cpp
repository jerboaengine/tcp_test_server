#include <iostream>
#include "tcpserver.h"
#include "log.h"

int main() {
    int ports[] = {23, 8000, 8001, 8002, 9001};
    for (int attemp = 0; attemp < 5; attemp++) {
        TCPServer server(ports[attemp]);
        try {
            server.startServer();
        } catch(const std::exception& e) {
            LOG(e.what());
        }
    }
    
    return 0;
}

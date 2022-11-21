#include "tcpserver.h"
#include "log.h"

#define MAXIMUM_PACKET_SIZE 500

TCPServer::TCPServer(const int port) {
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    servSocket = 0;
}

TCPServer::~TCPServer() {

}

void TCPServer::startServer() {
    if (initialize()) {
        handlingAcceptConnection();
    } else {
        throw std::runtime_error("failed to start server");
    }
}

bool TCPServer::initialize() {
    servSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servSocket <= 0) {
        LOG("failed to create socket");
        return false;
    }

    if (bind(servSocket, (const sockaddr*) &address, sizeof(sockaddr_in) ) < 0) {
        LOG("failed to bind socket");
        return false;
    }

    if(listen(servSocket, SOMAXCONN) < 0) {
        return false;
    }

    return true;
}

void TCPServer::handlingAcceptConnection() {
    while (true) {
        int socketDescr = accept(servSocket, NULL, NULL);
        if (socketDescr < 0) {
        } else {
            addSession(socketDescr);
            
        }
    }
}

void TCPServer::disconnectEvent(const int socketDescr) {
    const std::lock_guard<std::mutex> lock(mutex);
    removeSession(socketDescr);
    agent.removeSession(socketDescr);
}

void TCPServer::receiveData(const int sessionId, const char * const data, const unsigned int size, std::string &resp) {
    agent.handleReceiveData(sessionId, data, size, resp);
}

void TCPServer::addSession(const int socketDescr) {
    const std::lock_guard<std::mutex> lock(mutex);
    clients.emplace_back(std::make_unique<Client>(socketDescr, this));
}

void TCPServer::removeSession(const int socketDescr) {
    for(auto it = clients.begin(), end = clients.end(); it != end; ++it) {
        if ((*it)->getSocketDescr() == socketDescr) {
            clients.erase(it);
            break;
        }
    }
}

Client::Client(int socketDescr, ConnectionObserver *connObserver) 
: socketDescr(socketDescr), connObserver(connObserver) {
    listenThread = std::thread([this]{listenHandler();});
    listenThread.detach();
}

Client::~Client() {

}

void Client::listenHandler() {
    while (true) {
        char packetData[MAXIMUM_PACKET_SIZE];
        ssize_t receivedBytes = recv(socketDescr, (char*)&packetData, sizeof(packetData), 0);
        if (receivedBytes == -1 || receivedBytes == 0) {
            break;
        }

        if (connObserver != nullptr) {
            try {
                std::string internaResponse;
                connObserver->receiveData(getSocketDescr(), (char*)&packetData, receivedBytes, internaResponse);

                if (!internaResponse.empty()) {
                    send(socketDescr, internaResponse.c_str(), internaResponse.size(), 0);
                }
            } catch(const std::exception& e) {
                std::cerr << e.what() << '\n';
            }
        }
    }
    
    if (connObserver != nullptr) {
        connObserver->disconnectEvent(socketDescr);
    }
}

int Client::getSocketDescr() const {
    return socketDescr;
}

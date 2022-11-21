#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <netinet/in.h>
#include <thread>
#include <list>
#include <memory>
#include <exception>
#include "agent.h"

class ConnectionObserver {
public:
    virtual void disconnectEvent(const int socketDescr) = 0;
    virtual void receiveData(const int sessionId, const char * const data, const unsigned int size, std::string &resp) = 0;
    virtual ~ConnectionObserver() {}
};

class Client {
public:
    Client(int socketDescr, ConnectionObserver *connObserver = nullptr);
    ~Client();

    int getSocketDescr() const;

private:
    void listenHandler();

    ConnectionObserver *connObserver;
    int socketDescr;
    std::thread listenThread;
};

class TCPServer : public ConnectionObserver {
public:
    TCPServer(const int port);
    ~TCPServer();
    void startServer();

private:
    bool initialize();
    void handlingAcceptConnection();
    void addSession(const int socketDescr);
    void removeSession(const int socketDescr);

    void disconnectEvent(const int socketDescr) override;
    void receiveData(const int sessionId, const char * const data, const unsigned int size, std::string &resp) override;


    sockaddr_in address;
    int servSocket;
    std::mutex mutex;

    std::list<std::unique_ptr<Client>> clients;
    Agent agent;
};

#endif // TCPSERVER_H

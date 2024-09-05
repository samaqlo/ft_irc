# include "../inc/TcpSocket.hpp"
#include <netdb.h>
#include <strings.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

TcpSocket::TcpSocket()
{
    bzero(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
}

TcpSocket::~TcpSocket() {}

void TcpSocket::Socket(std::string port)
{
    (void) getaddrinfo(NULL, port.c_str(), &hints, &address);
    this->serverSocket = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (this->serverSocket == -1)
    {
        close(serverSocket);
        freeaddrinfo(address);
        throw std::runtime_error("Error: socket failed");
    }
    int reuseOn = 1;
    setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuseOn, sizeof(reuseOn));
    fcntl(this->serverSocket, F_SETFL, O_NONBLOCK);
}

void TcpSocket::Bind()
{
    if (bind(serverSocket, address->ai_addr, address->ai_addrlen) == -1)
    {
        close(serverSocket);
        freeaddrinfo(address);
        throw std::runtime_error("Error: bind failed");
    }
    freeaddrinfo(address);
}

void TcpSocket::Listen()
{
    if (listen(serverSocket, 128) == -1)
    {
        close(serverSocket);
        freeaddrinfo(address);
        throw std::runtime_error("Error: listen failed");
    }
}

int TcpSocket::Accept(char * ipAddr)
{
    int clientSocket;
    struct sockaddr_storage clientAddress;
    socklen_t clientLen = sizeof(clientAddress);

    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddress, &clientLen);
    if (clientSocket == -1)
        throw std::runtime_error("Error: accepting");

    fcntl(clientSocket, F_SETFL, O_NONBLOCK);

    // char ipAddr[50];

    sockaddr_in *sin = (sockaddr_in *) &clientAddress;
    inet_ntop(AF_INET, &sin->sin_addr, ipAddr, 50);
    
    std::cout << "Client " << ipAddr << " is connected" << std::endl;
    
    return clientSocket;
}

void TcpSocket::setServerSocket(int sock)
{
    this->serverSocket = sock;
}

int TcpSocket::getServerSocket()
{
    return this->serverSocket;
}
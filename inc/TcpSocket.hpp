#ifndef TCP_SOCKET
# define TCP_SOCKET

# include "Base.hpp"

class TcpSocket
{
    private:
        int serverSocket;
        struct addrinfo hints;
        struct addrinfo * address;
    public:
        TcpSocket();
        ~TcpSocket();

        void Socket(std::string port);
        void Bind();
        void Listen();
        int Accept(char * ipAddr);

        void setServerSocket(int sock);
        int getServerSocket();
};


#endif
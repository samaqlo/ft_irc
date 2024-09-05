# include "../inc/Server.hpp"
#include <ctime>

std::vector<Channel> Server::server_channels;
std::vector<Client> Server::clients;
clock_t Server::time_req = clock();

int main(int ac, char **av)
{
    try
    {
        if (ac != 3)
            throw std::runtime_error("Error: ./ircserv [port] [password] !!!");
        Server server(av[1], av[2]);
        server.serverSetup();
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}

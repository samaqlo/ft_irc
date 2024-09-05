#include "../../inc/Channel.hpp"
#include "../../inc/Server.hpp"
#include <vector>

bool    Channel::is_member(Client & clt)
{
    return (members[clt.getNickName()].second.getNickName() == clt.getNickName());
}

bool Channel::is_operator(Client & clt)
{
    //members[nickname].first 
    if(!is_member(clt))
        return false;
    return (members[clt.getNickName()].first == OP);
}

bool    is_channel(std::string & channel)
{
    std::vector<Channel>::iterator it = Server::server_channels.begin();
    for(; it != Server::server_channels.end(); it++)
    {
        if ((*it).get_Name() == channel)
            return true;
    }
    return false;
}

bool    is_client(std::string & nick)
{
    std::vector<Client>::iterator it = Server::clients.begin();
    for (;it != Server::clients.end(); it++)
    {
        if ((*it).getNickName() == nick)
            return true;
    }
    return false;
}

Client & find_client(const std::string & nick)
{
    std::vector<Client>::iterator it = Server::clients.begin();
    for (;it != Server::clients.end(); it++)
    {
        if ((*it).getNickName() == nick)
            return (*it);
    }
    return (*it);
}
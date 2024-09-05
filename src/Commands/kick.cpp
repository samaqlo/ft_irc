#include "../../inc/Command.hpp"
#include <map>
#include <sys/socket.h>
#include <vector>


void    eraseFromChannel(Channel & channel, Client & clt)
{
    std::map<std::string, int>::iterator it = clt.getClientChannels().begin();
    for(; it != clt.getClientChannels().end(); it++)
    {
        if (it->first == channel.get_Name())
        {
            clt.getClientChannels().erase(it);
            break;
        }
    }
    std::map<std::string, std::pair<int, Client> >::iterator it2 = channel.getMembers().begin();
    for(; it2 != channel.getMembers().end(); it2++)
    {
        if (it2->first == clt.getNickName())
        {
            channel.getMembers().erase(it2);
            break;
        }
    }
}

void    Command::kick()
{
    std::string reason = "";
    if (this->_cmd.size() < 3)
    {
        Replies::ERR_NEEDMOREPARAMS("KICK", this->clt);
        return ;
    }
    if(this->_cmd.size() > 3)
        reason = this->_cmd[3];
    if (!is_exist(this->_cmd[1]))
    {
        Replies::ERR_NOSUCHCHANNEL(this->_cmd[1], this->clt);
        return ;
    }
    Channel & channel = find_channel(this->_cmd[1]);
    if (!channel.is_member(this->clt))
    {
        Replies::ERR_NOTONCHANNEL(channel.get_Name(), this->clt);
        return ;
    }
    if (!channel.is_operator(this->clt))
    {
        Replies::ERR_CHANOPRIVSNEEDED(channel.get_Name(), this->clt);
        return ;
    }
    if(!is_client(this->_cmd[2]))
    {
        Replies::ERR_NOSUCHNICK(this->_cmd[2], this->clt);
        return ;
    }
    Client & target = find_client(this->_cmd[2]);
    if (!channel.is_member(target))
    {
        Replies::ERR_USERNOTINCHANNEL(target.getNickName(), channel.get_Name(), this->clt);
        return ;
    }
    else {
        eraseFromChannel(channel, target);
        if(reason.empty())
        {
            std::string message = ":" + this->clt.getNickName() + "!~" + this->clt.getUserName() + "@" + this->clt.getIp() + " KICK " + channel.get_Name() + " " + target.getNickName() + " :" + target.getNickName() + "\n";
            if (send(target.getFd(), message.c_str(), message.size(), 0) == -1)
                return ;
            channel.broadcast(message, false);
        }
        else
        {
            std::string message = ":" + this->clt.getNickName() + "!~" + this->clt.getUserName() + "@" + this->clt.getIp() + " KICK " + channel.get_Name() + " " + target.getNickName() + " :" + reason + "\n";
            if (send(target.getFd(), message.c_str(), message.size(), 0) == -1)
                return ;
            channel.broadcast(message, false);
        }
    }
}
#include "../../inc/Command.hpp"
#include <sys/socket.h>
#include <vector>

void    Command::invite()
{
    if(this->_cmd.size() < 3)
    {
        Replies::ERR_NEEDMOREPARAMS("INVITE", this->clt);
        return ;
    }
    if(!is_client(this->_cmd[1]))
    {
        Replies::ERR_NOSUCHNICK(this->_cmd[1], this->clt);
        return ;
    }
    Client & target = find_client(this->_cmd[1]);
    if (!is_exist(this->_cmd[2]))
    {
        Replies::ERR_NOSUCHCHANNEL(this->_cmd[2], this->clt);
        return ;
    }
    Channel & channel = find_channel(this->_cmd[2]);
    if(!channel.is_member(this->clt))
    {
        Replies::ERR_NOTONCHANNEL(channel.get_Name(), this->clt);
        return ;
    }
    if (channel.check_mode(INVITE_ONLY) && !channel.is_operator(this->clt))
    {
        Replies::ERR_CHANOPRIVSNEEDED(channel.get_Name(), this->clt);
        return ;
    }
    if (channel.is_member(target))
    {
        Replies::ERR_USERONCHANNEL(target.getNickName(), channel.get_Name(), this->clt);
        return ;
    }
    Replies::RPL_INVITING(target.getNickName(), channel.get_Name(), this->clt);
    std::string message2 = ":" + this->clt.getNickName() + "!~" + this->clt.getUserName() + "@" + this->clt.getIp() + " INVITE " + this->_cmd[1] + " :" + this->_cmd[2] + "\n";
    if (send(target.getFd(), message2.c_str(), message2.size(), 0) == -1)
        return;
    channel.setInviteList(target.getNickName());
}
#include "../../inc/Command.hpp"
#include <sys/socket.h>
#include <vector>

void    Command::topic()
{
    if (this->_cmd.size() < 2)
    {
        Replies::ERR_NEEDMOREPARAMS("TOPIC", this->clt);
        return ;
    }
    std::string topic = "";
    if (this->_cmd.size() > 2)
        topic = this->_cmd[2];
    if (!is_exist(this->_cmd[1]))
    {
        Replies::ERR_NOSUCHCHANNEL(this->_cmd[1], this->clt);
        return ;
    }
    Channel & chan = find_channel(this->_cmd[1]);
    if (!chan.is_member(this->clt))
    {
        Replies::ERR_NOTONCHANNEL(chan.get_Name(), this->clt);
        return ;
    }
    if (chan.check_mode(ISTOPIC) && !chan.is_operator(this->clt))
    {
        Replies::ERR_CHANOPRIVSNEEDED(chan.get_Name(), this->clt);
        return ;
    }
    if(this->_cmd.size() == 2)
    {
        if(chan.getTopicContent().empty())
        {
            Replies::RPL_NOTOPIC(chan.get_Name(), this->clt);
            return ;
        }
        else
        {
            Replies::RPL_TOPIC(chan, this->clt);
            Replies::RPL_TOPICWHOTIME(chan, this->clt);
            return ;
        }
    }
    else
    {
        chan.setTopic(this->clt.getNickName(), topic, time(0));
        chan.broadcast(":" + this->clt.getNickName() + "!~" + this->clt.getUserName() + "@" + this->clt.getIp() + " TOPIC " + chan.get_Name() + " :" + topic + "\n", false);
    }
}

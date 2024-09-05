#include "../../inc/Command.hpp"
#include "../../inc/Replies.hpp"
#include <algorithm>
#include <map>
#include <string>
#include <sys/socket.h>
#include <utility>
#include <vector>

bool checkDuplicates(std::vector<std::pair<std::string, bool> > &targets, std::string &toCheck)
{
    std::vector<std::pair<std::string, bool> >::iterator it = targets.begin();
    for (; it!=targets.end(); it++)
    {
        if(it.base()->first == toCheck)
            return false;
    }
    return true;
}

std::vector<std::pair<std::string, bool> >  splitTargets(std::string & cmd, char del)
{
    std::string s;
    std::stringstream ss(cmd);
    std::vector<std::pair<std::string, bool> > ret;

    while(std::getline(ss, s, del))
    {
        if (!s.empty())
        {
            if (!checkDuplicates(ret, s))
                continue;
             ret.push_back(std::make_pair(s, false));
        }
    }
    return (ret);
}

bool    multipleTargets(const std::string & target)
{
    if (target.find(',') != std::string::npos)
        return (true);
    return (false);
}

void    sendToChannel(const std::string &message, Channel &channel, Client &clt)
{
    std::map<std::string,std::pair<int, Client> >::iterator it = channel.getMembers().begin();
    for(;it != channel.getMembers().end();it++)
    {
        if ((*it).first == clt.getNickName())
            continue;
        if (send(channel.getMembers()[(*it).first].second.getFd(), message.c_str(), message.length(), 0) == -1)
            return ;
    }
}

int    distributeMsg(std::pair<std::string, bool> & target, std::string & text, Client &clt)
{
    if(target.first[0] != '#')
    {
        if (!is_client(target.first))
        {
            Replies::ERR_NOSUCHNICK(target.first, clt);
            return 0;
        }
        else if (target.second == false)
        {
            Client & targetClient = find_client(target.first);
            std::string message = ":" + clt.getNickName() + "!~" + clt.getUserName() + "@" + clt.getIp() + " PRIVMSG " + target.first + " :" + text + "\n";
            if (send(targetClient.getFd(), message.c_str(), message.size(), 0) == -1)
                return -1;
            target.second = true;
            return 1;
        }
    }
    else {
        if (!is_channel(target.first))
        {
            Replies::ERR_NOSUCHNICK(target.first, clt);
            return  0;
        }
        else if (target.second == false)
        {
            Channel & targetChannel = find_channel(target.first);
            if (!targetChannel.is_member(clt))
            {
                Replies::ERR_CANNOTSENDTOCHAN(targetChannel.get_Name(), clt);
                return 0;
            }
            std::string message = ":" + clt.getNickName() + "!~" + clt.getUserName() + "@" + clt.getIp() + " PRIVMSG " + target.first + " :" + text + "\n";
            sendToChannel(message, targetChannel, clt);
            target.second = true;
            return 1;
        }
    }
    return 1;
}

void    Command::privmsg()
{
    if (this->_cmd.size() == 1)
    {
        Replies::ERR_NORECIPIENT("PRIVMSG", this->clt);
        return ;
    }
    if (this->_cmd.size() == 2)
    {
        Replies::ERR_NOTEXTTOSEND(this->clt);
        return ;
    }
    if(multipleTargets(this->_cmd[1]))
    {
        std::vector<std::pair<std::string, bool> > targets = splitTargets(this->_cmd[1], ',');
        for (std::vector<std::pair<std::string, bool> >::iterator it = targets.begin(); it != targets.end(); it++)
        {
            if(distributeMsg(*it, this->_cmd[2], this->clt) == 0)
                break;
        }
    }
    else
    {
        std::pair<std::string, bool> target(this->_cmd[1], false);
        distributeMsg(target, this->_cmd[2], this->clt);
    }
}
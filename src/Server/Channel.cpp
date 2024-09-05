#include "../../inc/Channel.hpp"
#include <cstddef>
#include <ctime>
#include <utility>

std::map<std::string,std::pair<int, Client> > members;

Channel::Channel()
{

}

Channel::Channel(std::string chan, std::string key)
{
    channel_name = chan;
    mode = 0;
    this->key = key;
    topicContent = "";
    topicCreator = "";
    topicTime = 0;
}

void    Channel::set_member(Client & clt, int role)
{
    members[clt.getNickName()] = std::make_pair(role,clt);
}

void    Channel::set_name(const std::string & name)
{
    this->channel_name = name;
}

void    Channel::set_key(const std::string & key)
{
    this->key = key;
}

void    Channel::set_mode(int mode)
{
    this->mode = mode | this->mode;
}

void    Channel::unset_mode(int mode)
{
    int mask = ~(1 << mode);
    this->mode = this->mode & mask;
}
const std::string & Channel::get_Name() const
{
    return (channel_name);
}

const std::string & Channel::get_Key() const
{
    return (key);
}


int Channel::get_Mode() const
{
    return (mode);
}

int Channel::check_mode(int to_check)
{
    return (mode & to_check);
}

void    Channel::broadcast(std::string  msg, bool op)
{
    std::map<std::string,std::pair<int, Client> >::iterator it = members.begin();
    for(;it != members.end();it++)
    {
        if (op && (*it).second.first != OP)
            continue;
        if (send(members[(*it).first].second.getFd(), msg.c_str(), msg.length(), 0) == -1)
            return ;
    }
}

std::string    Channel::printMembers()
{
    std::string concatMembers;
    std::map<std::string,std::pair<int, Client> >::iterator it = members.begin();
    for(;it != members.end();it++)
    {
        if (it != members.begin())
            concatMembers += " ";
        if ((*it).second.first == OP)
            concatMembers += "@" + (*it).first; 
        else
            concatMembers += (*it).first;
    }
    return (concatMembers);
}

void    Channel::setTopic(const std::string & nick, const std::string & content, time_t time)
{
    topicContent = content;
    topicCreator = nick;
    topicTime = time;
}

std::string Channel::getTopicCreator() const
{
    return (topicCreator);
}

std::string Channel::getTopicContent() const
{
    return (topicContent);
}

time_t Channel::getTopicTime() const
{
    return (topicTime);
}

std::map<std::string,std::pair<int, Client> > & Channel::getMembers()
{
    return (members);
}

void    Channel::del_member(const std::string & nick)
{
    members.erase(nick);
}

int     Channel::get_num_members()
{
    if (members.empty())
        return 0;
    return ((int)members.size());
}

void    Channel::setInviteList(const std::string & nick)
{
    inviteList.push_back(nick);
}

std::vector<std::string> & Channel::getInviteList()
{
    return (inviteList);
}

void    Channel::set_operator(std::string & nick, int Membership)
{
    members[nick].first = Membership;
}

bool    Channel::is_client(const std::string & nick)
{
  std::map<std::string,std::pair<int, Client> >::iterator it =  members.find(nick);
  if (it != members.end())
    return true;
return false;
}

void    Channel::set_limit(int lim)
{
    this->limit = lim;
}

int Channel::get_limit()
{
    return (limit);
}

bool    Channel::is_invited(std::string nick)
{
    for (size_t i = 0; i < inviteList.size(); i++) {
        if (inviteList[i] == nick)
            return true;
    }
    return false;
}

void    Channel::set_new_nick(std::string oldNick, const std::string  newNick)
{
    std::map<std::string,std::pair<int, Client> >::iterator it = members.begin();
    Client tmp;
    int tmp2;
    for (; it != members.end();it++) {
        if ((*it).first == oldNick)
        {
            (*it).second.second.setClientNick(newNick);
            tmp = (*it).second.second;
            tmp2 = (*it).second.first;
            members.insert(std::make_pair(newNick, std::make_pair(tmp2, tmp)));
            members.erase(oldNick);
            return;
        }
    }

}
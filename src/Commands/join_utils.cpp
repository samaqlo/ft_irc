#include "../../inc/Command.hpp"
#include <cstddef>

int restrictions(const std::string & name)
{
    return (name.find(32, 0) || name.find(7, 0) || name.find(',', 0));
}

std::string  _to_string(long long num)
{
    std::stringstream ss;
    ss << num;
    return (ss.str());
}

void    create_channel(std::map<std::string,std::string>::iterator it, Client & clt)
{
    Channel new_channel((*it).first, (*it).second);
    new_channel.set_member(clt, OP);
    clt.set_clt_chans(new_channel.get_Name(), OP);
    Server::server_channels.push_back(new_channel);
}

bool    join_channel(std::map<std::string,std::string>::iterator it, Client & clt)
{
    std::vector<Channel>::iterator _it =  Server::server_channels.begin();
    
    for (;_it != Server::server_channels.end(); _it++)
    {
        if ((*_it).get_Name() == (*it).first)
        {
            if ((*_it).check_mode(LIMIT) && (*_it).get_num_members() - 1 == (*_it).get_limit())
                    return (Replies::ERR_CHANNELISFULL((*it).first, clt), false);
            if ((*_it).check_mode(INVITE_ONLY) && !(*_it).is_invited(clt.getNickName()))
                return (Replies::ERR_INVITEONLYCHAN((*it).first, clt), false);
            
            if((*_it).check_mode(PASSWORD) && !(*_it).check_mode(INVITE_ONLY)) 
                {
                    if ((*_it).get_Key() == (*it).second)
                        (*_it).set_member(clt, REG);
                    else
                        return (Replies::ERR_BADCHANNELKEY((*it).first, clt), false);
                }
            else
                (*_it).set_member(clt, REG);
        }
    }
    clt.set_clt_chans((*it).first, REG);
    return (true);
}

int is_exist(const std::string & name)
{
    std::vector<Channel>::iterator it =  Server::server_channels.begin();
    for (;it != Server::server_channels.end(); it++)
    {
        if ((*it).get_Name() == name)
            return (1);
    }
    return 0;
}

Channel & find_channel(const std::string & name)
{
    std::vector<Channel>::iterator it =  Server::server_channels.begin();

    for (;it != Server::server_channels.end(); it++)
    {
        if ((*it).get_Name() == name)
            break;
    }
    return (*it);
}

int channel_index(const std::string & name)
{
    int i = 0;

    for (; i < (int)Server::server_channels.size(); i++) {
        if (Server::server_channels[i].get_Name() == name)
            return (i);
    }
    return (i);
}

std::vector<std::string>  split(std::string & cmd, char del)
{
    std::string s;
    std::stringstream ss(cmd);
    std::vector<std::string> ret;

    while(std::getline(ss, s, del)) 
        ret.push_back(s);
    return (ret);
}

int validate_name(const std::string & name)
{
    if (name.size() == 1 && name.at(0) == '0')
        return (0);
    if (name.at(0) != '#' || name.length() > 200 || !restrictions(name))
        return (403);
    return (0);
}

std::map<std::string, std::string>  parse_args(std::vector<std::string> args, Client & clt)
{
    std::vector<std::string> store_chan;
    std::vector<std::string> store_key;
    std::map<std::string, std::string> ret;
    std::map<std::string, std::string>::iterator it;
        if (args.size() > 3 || args.size() < 2)
            return (Replies::ERR_NEEDMOREPARAMS(args[0], clt), ret);
        if (args.size() == 2)
            args.push_back("");
        store_chan = split(args[1], ',');
        if(!args[2].empty())
            store_key = split(args[2], ',');
        if (store_key.size() > store_chan.size())
            return (Replies::ERR_NEEDMOREPARAMS(args[0], clt), ret);
        for (int i = 0; i < (int)store_key.size(); i++)
            ret[store_chan[i]] = store_key[i];
        for(int i = store_key.size(); i < (int)store_chan.size(); i++)
             ret[store_chan[i]] = "";
        for(it = ret.begin(); it != ret.end(); it++)
        {
            if (validate_name((*it).first) == 403)
            {
                Replies::ERR_BADCHANMASK((*it).first, clt);
                ret.erase((*it).first);
                it = ret.begin();
                if (it == ret.end())
                    return (ret);
            }
        }
    return (ret);
}

void    send_reply(std::string  msg, Client  clt)
{
    msg += "\r\n";
    if (send(clt.getFd(), msg.c_str(), msg.length(), 0) == -1)
        return ;
}

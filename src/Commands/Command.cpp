#include "../../inc/Command.hpp"
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <system_error>

const char *Command::urls[] = {"tAGnKpE4NCI", "dQw4w9WgXcQ", "KMfL7rVAu0U", "03YWncM1GR4", "P6QOr3SdE_Q"};

Command::Command(const std::vector<std::string>  & cmd, Client & clt) : _cmd(cmd), clt(clt)
{
    cmd_id = -1;
}

void    Command::join()
{
    std::map<std::string,std::string> chan_key;
    Channel channels[(int)chan_key.size()];
    std::map<std::string,std::string>::iterator it;
    Channel tmp_chan;

    chan_key = parse_args(_cmd, clt);
    if (chan_key.empty())
        return;
    it = chan_key.begin();
    for(int i = 0; i < (int)chan_key.size();i++)
    {
        if ((int)(*it).first.size() == 1 && (*it).first.at(0) == '0')
        {
            Server::quit_channels(clt, false);
            return;
        }
        if (is_exist((*it).first))
        {
            if (find_channel((*it).first).is_member(clt))
                return ;
            if (!join_channel(it, clt))
             return ;
        }
        else
            create_channel(it, clt);
        tmp_chan = find_channel((*it).first);
        tmp_chan.broadcast(":" + clt.getNickName() + "!~" + clt.getUserName() + "@" + clt.getIp() + " JOIN " + (*it).first + "\r\n", false);
        if (!tmp_chan.getTopicContent().empty())
        {
            Replies::RPL_TOPIC(tmp_chan, clt);
            Replies::RPL_TOPICWHOTIME(tmp_chan, clt);
        }
        Replies::RPL_NAMREPLY(tmp_chan, clt);
        Replies::RPL_ENDOFNAMES(tmp_chan, clt);
        it++;
    }
}

void    Command::mode()
{

    if (!is_exist(_cmd[1]))
    {
        Replies::ERR_NOSUCHCHANNEL(_cmd[1], clt);
        return;
    }
    if (clt.getMembership(_cmd[1]) != OP)
    {
        Replies::ERR_CHANOPRIVSNEEDED(_cmd[1], clt);
        return ;
    }
    if (_cmd.size() == 2)
    {
        Replies::RPL_CHANNELMODEIS(_cmd[1], clt);
        return;
    }
    check_modestring(_cmd, clt);
}

void    Command::identify_cmd()
{
    cmd_id = (_cmd[0] == "PASS") * 1 + (_cmd[0] == "USER") * 2 + (_cmd[0] == "NICK") * 3\
    + (_cmd[0] == "JOIN") * 4 + (_cmd[0] == "MODE") * 5 + (_cmd[0] == "KICK") * 6 + (_cmd[0] == "INVITE") * 7 + (_cmd[0] == "TOPIC") * 8\
    + (_cmd[0] == "PRIVMSG") * 9 + (_cmd[0] == "BOT") * 10 ;
}

void    Command::invoke_cmd()
{
    switch(cmd_id)
    {
        case JOIN:
            join();
            break;
        case MODE:
            mode();
            break;
        case TOPIC:
            topic();
            break;
        case KICK:
            kick();
            break;
        case INVITE:
            invite();
            break;
        case PRIVMSG:
            privmsg();
            break;
        case BOT:
            bot();
            break;
    };
}
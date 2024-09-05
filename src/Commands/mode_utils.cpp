#include "../../inc/Command.hpp"
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <utility>
#include <vector>

//funcs mode k, mode i, mode l, mode t, 


bool    modeKey(Channel & channel, std::vector<std::string> & args, Client & clt, bool action)
{
    if (args.size() == 0)
        return (Replies::ERR_NEEDMOREPARAMS("MODE", clt), false);
    if (action)
        {
            if (channel.check_mode(PASSWORD))
                return (Replies::ERR_KEYALREADYSET(channel.get_Name(), clt), false);
            channel.set_mode(PASSWORD);
            channel.set_key(args[0]);
            args.erase(args.begin());
        }
    else {
            if (!channel.check_mode(PASSWORD))
                return (Replies::ERR_KEYALREADYSET(channel.get_Name(), clt), false);
            if (args[0] != channel.get_Key())
                return (Replies::ERR_KEYALREADYSET(channel.get_Name(), clt), false);
            channel.unset_mode(UPASSWORD);
            channel.set_key("");
            args.erase(args.begin());
    }
    return (true);
}

bool    modeInvite(Channel & channel, bool action)
{
    if (action)
    {
        if (channel.check_mode(INVITE_ONLY))
            return false;
        channel.set_mode(INVITE_ONLY);
    }
    else {
        if (!channel.check_mode(INVITE_ONLY))
            return (false);
        channel.unset_mode(UINVITEONLY);
    }

    return (true);
}

bool    modeTopic(Channel & channel, bool action)
{
    if (action)
    {
        if (channel.check_mode(ISTOPIC))
            return false;
        channel.set_mode(ISTOPIC);
    }
    else {
        if (!channel.check_mode(ISTOPIC))
            return false;
        channel.unset_mode(UTOPIC);
    }
    return (true);
}

bool    is_nick(std::string & nick)
{
    std::vector<Client>::iterator it = Server::clients.begin();

    for (; it != Server::clients.end(); it++) {
        if ((*it).getNickName() == nick)
            return true;
    }
    return false;
}

bool    modeOperator(Channel & channel, Client & clt, std::vector<std::string> & args,bool action)
{
    if (args.size() == 0)
            return (Replies::ERR_NEEDMOREPARAMS("MODE", clt), false);
    if (!is_nick(args[0]))
        return (Replies::ERR_NOSUCHNICK(args[0], clt), false);
    if (!channel.is_client(args[0]))
        return (Replies::ERR_USERNOTINCHANNEL(channel.get_Name(), args[0], clt), false);
    Client & target = find_client(args[0]);
    if (action)
    {
        if (channel.is_operator(target))
            return false;
        target.EditMembership(channel.get_Name(), OP);
        channel.set_operator(args[0], OP);
        args.erase(args.begin());
    }
    else {
        if (!channel.is_operator(target))
            return false;
        target.EditMembership(channel.get_Name(), REG);
        channel.set_operator(args[0], REG);
        args.erase(args.begin());
    }
    return (true);
}

bool parse_limit(std::string & lim)
{
    std::string nums("0123456789");
    long long limit;
    for (int i = 0; i < (int)lim.length(); i++) {
        if (nums.find(lim.at(i), 0) == nums.npos)
            return  false;
    }
    limit = atoi(lim.c_str());
    if (lim != _to_string(limit))
        return false;
    return true;
}
bool    modeLimit(Channel & channel, std::vector<std::string> & args, bool action, Client & clt)
{
    if (args.size() == 0 || !parse_limit(args[0]))
        return (Replies::ERR_NEEDMOREPARAMS("MODE", clt), false);
    if (action)
    {
        channel.set_mode(LIMIT);
        channel.set_limit(atoi(args[0].c_str()));
        args.erase(args.begin());
    }
    else {
        channel.unset_mode(ULIMIT); 
    }
    return (true);
}

bool    addMode(char mode, Channel & channel,std::vector<std::string> & args, Client & clt)
{
    bool status;
    int casemode;
        casemode = (mode == 'k') * 1 + (mode == 'i') * 2 + (mode == 't') * 3 + (mode == 'o') * 4 + (mode == 'l') * 5;
    switch (casemode) {
        case key:
            status = modeKey(channel, args, clt, true);
            break;
        case invite:
            status = modeInvite(channel, true);
            break;
        case topic:
            status = modeTopic(channel, true);
            break;
        case opera:
            status = modeOperator(channel, clt, args, true);
            break;
        case lim:
            status = modeLimit(channel, args, true, clt);
            break;
        default:
            Replies::ERR_UNKNOWNMODE(mode, clt);
            return false;
    }
    return status;
}

bool    rmMode(char mode, Channel & channel,std::vector<std::string> & args, Client & clt)
{
    bool status;
    int casemode;
        casemode = (mode == 'k') * 1 + (mode == 'i') * 2 + (mode == 't') * 3 + (mode == 'o') * 4 + (mode == 'l') * 5;
    switch (casemode) {
        case key:
            status = modeKey(channel, args, clt, false);
            break;
        case invite:
            status = modeInvite(channel, false);
            break;
        case topic:
            status = modeTopic(channel, false);
            break;
        case opera:
            status = modeOperator(channel, clt, args, false);
            break;
        case lim:
            status = modeLimit(channel, args, false, clt);
            break;
        default:
            Replies::ERR_UNKNOWNMODE(mode, clt);
            return false;
    }
    return status;
}

void    extract_modes(std::string & modestring, std::vector<std::pair<bool, char> > & modestrings)
{
    bool flag = true;
    for (int i = 0; i < (int)modestring.size(); i++) {
        if (modestring[i] == '+')
            flag = true;
        else if (modestring[i] == '-')
            flag = false;
        else if (flag == true)
            modestrings.push_back(std::make_pair(flag, modestring[i]));
        else if (flag == false)
            modestrings.push_back(std::make_pair(flag, modestring[i]));
    }
}

typedef struct s_success
{
    char    mode;
    char    sign;
} t_success;

std::string success_str(std::vector<std::pair<t_success, std::string> > success)
{
    if (success.size() == 0)
        return "";
    std::string ret;
    std::vector<std::pair<t_success, std::string> >::iterator it = success.begin();
    for (; it != success.end();it++)
    {
        ret.push_back((*it).first.sign);
        ret.push_back((*it).first.mode);
    }
    for (it = success.begin(); it != success.end(); it++)
    {
        ret.push_back(32);
        ret += (*it).second;
    }
    return ret;
}

bool    check_modestring(std::vector<std::string> args, Client & clt)
{
    std::vector<std::pair<bool, char> > modestrings;
    std::vector<std::pair<t_success, std::string> > success;
    extract_modes(args[2], modestrings);
    std::vector<std::pair<bool, char> >::iterator it = modestrings.begin();
    std::vector<std::string> modesArgs;
    std::string tmp_arg;
    t_success tmp;
    Channel & channel = find_channel(args[1]);
    modesArgs.insert(modesArgs.begin(),args.begin() + 3, args.end());
    for (; it != modestrings.end();it++) {
            if (modesArgs.size() != 0)
                tmp_arg = modesArgs[0]; 
        if ((*it).first == true)
        {
            if (addMode((*it).second, channel,modesArgs, clt))
                {
                    tmp.mode = (*it).second;
                    tmp.sign = '+';
                    if (tmp.mode == 't' || tmp.mode == 'i')
                        tmp_arg = "";
                    success.push_back(std::make_pair(tmp, tmp_arg));
                }
        }
        else if ((*it).first == false)
        {
            if (rmMode((*it).second, channel,modesArgs, clt))
                {
                    tmp.mode = (*it).second;
                    tmp.sign = '-';
                    if (tmp.mode == 't' || tmp.mode == 'i' || tmp.mode == 'l')
                        tmp_arg = "";
                    success.push_back(std::make_pair(tmp, tmp_arg));
                }
        }
    }       
    if (success_str(success) == "")
        return false;
    channel.broadcast(":" + clt.getNickName() + "!~" + clt.getUserName() + "@" + clt.getIp() + " MODE " + channel.get_Name() + " " + success_str(success)  + "\r\n", false);
    return (true);
}


int    check_modes(const std::string & str)
{
    char mode = str.at(str.find('+', 0) + 1);
    
    return ((mode == 'i') * 1 + (mode == 't') * 2 + (mode == 'k') * 3 + (mode == 'o') * 4 + (mode == 'l') * 5);
}
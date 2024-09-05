# include "../../inc/Server.hpp"
#include <cctype>
#include <system_error>

// Save nickname after authenticate
// nickname parsing
bool fisttime = true;

void Server::nick(int &clientIdx)
{
    if (!clients[clientIdx].isValidPAss())
    {
        Replies::notRegistredMsg(clients[clientIdx]);
        return ;
    }
    if (parseNickName(this->cmd[1], clientIdx))
    {
        if(fisttime)
            fisttime = false;
        if (!fisttime)
            {
                clients[clientIdx].EditNick(clients[clientIdx].getNickName(), cmd[1]);
            }
        clients[clientIdx].setClientNick(this->cmd[1]);
        nicknames[clientIdx] = this->cmd[1];
        clients[clientIdx].setNickFlagTrue();
    }
}

bool disallowedChar(std::string &nick)
{
    std::string allowed_charachters = "-|[]`\\^{}_";
    if (!isalpha(nick[0]))
        return false;
    for (size_t idx = 0; idx < nick.size(); idx++)
    {
        if (allowed_charachters.find(nick[idx]) > allowed_charachters.size() && !isdigit(nick[idx]) && !isalpha(nick[idx]))
            return false;
    }
    return true;
}

bool Server::parseNickName(std::string &nick, int &clientIdx)
{
    if (nickAlreadyExist(nick, clientIdx))
    {
        Replies::AlreadyInUseMsg(nick, clients[clientIdx]);
        return false;
    }
    else if (this->cmd.size() == 1 || this->cmd[1].empty())
    {
        Replies::NoNickGivenMsg(clients[clientIdx]);
        return false;
    }
    else if (!disallowedChar(nick))
    {
        Replies::ErroneusNickMsg(nick, clients[clientIdx]);
        return false;
    }
    return true;
}

bool Server::nickAlreadyExist(std::string &nick, int &clientIdx)
{
    std::vector<std::string>::iterator it = std::find(nicknames.begin(), nicknames.end(), nick);
    if (it != nicknames.end() && *it != clients[clientIdx].getNickName())
        return true;
    return false;
}

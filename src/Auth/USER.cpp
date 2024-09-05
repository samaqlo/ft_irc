# include "../../inc/Server.hpp"

void Server::user(int &clientIdx)
{
    if (!clients[clientIdx].isValidPAss())
    {
        Replies::notRegistredMsg(clients[clientIdx]);
        return ;
    }
    if (this->cmd.size() < 5)
    {
        Replies::ERR_NEEDMOREPARAMS("USER", clients[clientIdx]);
        return ;
    }
    clients[clientIdx].setUserName(this->cmd[1]);
    clients[clientIdx].setHostName(this->cmd[2]);
    clients[clientIdx].setServName(this->cmd[3]);
    clients[clientIdx].setRealName(this->cmd[4]);
    clients[clientIdx].setUserFlagTrue();
}

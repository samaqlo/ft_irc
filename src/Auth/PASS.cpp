# include "../../inc/Server.hpp"

void Server::pass(int &clientIdx)
{
    if (this->cmd.size() < 2)
    {
        Replies::ERR_NEEDMOREPARAMS("PASS", clients[clientIdx]);
        return ;
    }
    clients[clientIdx].setClientPass(this->cmd[1]);
    if (clients[clientIdx].getPassword() != password && !clients[clientIdx].isValidPAss())
    {
        Replies::incorectPassMsg(clients[clientIdx]);
        return ;
    }
    clients[clientIdx].setPassFlagTrue();
}

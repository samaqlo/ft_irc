# include "../../inc/Server.hpp"
#include "../../inc/Command.hpp"
#include <__errc>
#include <algorithm>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/signal.h>
#include <vector>

Server::Server() {}

Server::Server(const std::string &port, const std::string &password) : port(port), password(password)
{
    isValidCmd = true;
    flag = 0;
    parseArgs();
}

void Server::parseArgs()
{
    std::stringstream ss(port);
    int port;

    if (!(ss>>port) || port > 65535 || port < 1024)
        throw std::runtime_error("Error");

    ss.clear();

    if (password.empty())
        throw std::runtime_error("Error");
}

void Server::serverSetup()
{
    serverSock.Socket(this->port);
    serverSock.Bind();
    serverSock.Listen();
    startAccepting();
}

void Server::startAccepting()
{
    struct pollfd fd;
    fd.fd = serverSock.getServerSocket();
    fd.events = POLLIN;
    fds.push_back(fd);
    while (true)
    {
        signal(SIGPIPE, SIG_IGN);
        int polling = poll(&fds[0], fds.size(), 0);
        if (polling == -1)
            throw std::runtime_error("error");
        for (size_t idx = 0; idx < fds.size(); idx++)
        { 
            if (fds[idx].revents & POLLIN)
            {
                ChannelMonitoring();
                if (!idx)
                    acceptClient(fd);
                else
                    receiveMsg(idx);
            }
        }
    }
}

void Server::acceptClient(struct pollfd &fd)
{
    char ipAddr[50];
    int clientSocket = serverSock.Accept(ipAddr);

    bzero(&fd, sizeof(fd));
    fd.fd = clientSocket;
    fd.events = POLLIN;

    fds.push_back(fd);

    clients.push_back(Client(clientSocket, ipAddr));

    nicknames.push_back("*");
}

void Server::receiveMsg(int idx)
{
    char recvMsg[MESSAGELEN];
    bzero(recvMsg, MESSAGELEN);
    int receive = recv(fds[idx].fd, recvMsg, MESSAGELEN, 0);
    if (receive == -1)
        return;
    if (receive == 0)
    {
        std::cout << "Client disconnected" << std::endl;
        quit_channels(clients[idx - 1], true);
        close(fds[idx].fd);
        clients.erase(clients.begin() + idx - 1);
        nicknames.erase(nicknames.begin() + idx - 1);
        fds.erase(fds.begin() + idx);
        return ;
    }
    else if (receive <= MESSAGELEN)
    {
        std::string str(recvMsg, receive);
        for (int i = 0; i < receive; i++)
        {
            clients[idx - 1].appendMessage(str[i]);
            if (str[i] == '\n')
            {
                handleCommand(clients[idx - 1].getMessage(), idx - 1);
                clients[idx - 1].clearBuffer();
                cmd.clear();
            }
        }
    }
}

void remove_clrf(std::string & cmd)
{
    cmd = (cmd[cmd.size() - 2] == '\r' && cmd[cmd.size() - 1] == '\n') ? cmd.substr(0, cmd.size() - 2) : cmd.substr(0, cmd.size() - 1);
}

void Server::handleCommand(const std::string &command, int clientIdx)
{
    if (command.size() > 512)
    {
        send_reply(SERVER_NAME " 417 " + clients[clientIdx].getNickName() + " :Line too long", clients[clientIdx]);
        return ;
    }
    std::string cmd(command);
    remove_clrf(cmd);
    if (!parseCommand(cmd, clientIdx) || !cmd.size())
        return ;
    if (!clients[clientIdx].isAuthentified())
    {
        authentification(clientIdx);
        if (clients[clientIdx].authDone())
            authentificationHelper(clientIdx);
    }
    else
    {
        if (this->cmd[0] == "PASS" || this->cmd[0] == "USER")
        {
            Replies::mayNotReregisterMsg(clients[clientIdx]);
            return ;
        }
        else if (this->cmd[0] == "NICK")
            nick(clientIdx);
        Command cmd(this->cmd, clients[clientIdx]);
        cmd.identify_cmd();
        cmd.invoke_cmd();
    }
}

bool Server::checkPassCorrect(std::string ServerPAss, std::string CLientPass)
{
    if (ServerPAss == CLientPass)
        return true;
    return false;
}

void Server::authentification(int &clientIdx)
{
    if (this->cmd[0] == "PASS")
        pass(clientIdx);
    else if (this->cmd[0] == "NICK")
        nick(clientIdx);
    else if (this->cmd[0] == "USER")
        user(clientIdx);
    else
        Replies::notRegistredMsg(clients[clientIdx]);
}

void Server::authentificationHelper(int &clientIdx)
{
    clients[clientIdx].setAuthFlag();
    Replies::welcomeMsg(clients[clientIdx]);
}

bool Server::parseCommand(const std::string &command, int clientIdx)
{ 
    std::string cmd(command);
    while (true)
    {
        if (cmd.empty())
            return false;
        size_t spacePos = cmd.find(' ');
        if ((spacePos > cmd.size() || spacePos < 0))
        {
            (cmd[0] == ':') ? this->cmd.push_back(cmd.substr(1, cmd.size())) : this->cmd.push_back(cmd.erase(cmd.size()));
            break;
        }
        std::string sub = cmd.substr(0, spacePos);
        if (sub.size() >= 1 && sub != " ")
        {
            if (sub == ":" || sub[0] == ':')
            {
                this->cmd.push_back(cmd.substr(1, cmd.size()));
                break;
            }
            this->cmd.push_back(sub);
        }
        cmd = cmd.substr(spacePos + 1, cmd.size());
    }
    return (commandValidation(clientIdx));
}

void fillVec(std::vector<std::string> &known)
{
    known.push_back("PASS");
    known.push_back("USER");
    known.push_back("NICK");
    known.push_back("JOIN");
    known.push_back("MODE");
    known.push_back("KICK");
    known.push_back("INVITE");
    known.push_back("TOPIC");
    known.push_back("PRIVMSG");
    known.push_back("NOTICE");
    known.push_back("PART");
    known.push_back("BOT");
}

bool Server::commandValidation(int &clientIdx)
{
    std::vector<std::string> known;

    fillVec(known);
    if (std::find(known.begin(), known.end(), this->cmd[0]) == known.end())
    {
        Replies::unknownCmdMsg(this->cmd[0],clients[clientIdx]);
        return (false);
    }
    return (true);
}

Server::~Server() {}

//GETERS//
std::string Server::getPort()
{
    return (this->port);
}

std::string Server::getPasswd()
{
    return (this->password);
}
std::vector<Channel>::iterator    Server::erase_channel(const std::string & name)
{
    std::vector<Channel>::iterator it = server_channels.begin();
    if (server_channels.size() == 0)
        return it;
    for(; it != server_channels.end(); ++it)
    {
        if ((*it).get_Name() == name)
        {
            it = server_channels.erase(it);
            return it;
        }
    }
    return  it;
}

void    Server::quit_channels(Client & clt, bool flag)
{
    std::map<std::string, int>::iterator it = clt.getClientChannels().begin();
    for(;it != clt.getClientChannels().end(); it++)
    {
        if (flag)
            find_channel((*it).first).broadcast(":" + clt.getNickName() + "!~" + clt.getUserName() + "@" + clt.getIp() + " QUIT :EOF from client\r\n", false);
        if (!flag)
            find_channel((*it).first).broadcast(":" + clt.getNickName() + "!~" + clt.getUserName() + "@" + clt.getIp() + " " + find_channel((*it).first).get_Name() + " :Left all channels\r\n", false);
        find_channel((*it).first).del_member(clt.getNickName());
        if (find_channel((*it).first).get_num_members() == 0)
            erase_channel((*it).first);
    }
}

std::string Server::get_requested_time()
{
    clock_t now = clock() - time_req;

    std::string now_str;
    std::stringstream ss;
    
    ss << (float)now/CLOCKS_PER_SEC;
    ss >> now_str;
    return now_str;
}

void    Server::ChannelMonitoring()
{
    std::vector<Channel>::iterator it = server_channels.begin();
    if(server_channels.size() == 0)
        return;
    for(;it != server_channels.end(); it++) {
        if ((*it).get_num_members() == 0)
        {
            it = erase_channel((*it).get_Name());
            if (it == server_channels.end())
                return;
        }
    }
}
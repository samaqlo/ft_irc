# include "../../inc/Client.hpp"
#include <__errc>
#include <cstring>
#include <netdb.h>
#include <utility>

Client::Client()
{

}

Client::Client(int clientSocket, std::string ipAddr) : socket_fd(clientSocket)
{
    this->Authentified = false;
    this->ValidPAss = false;
    this->ValidNick = false;
    this->ValidUser = false;
    this->flag = 0;
    this->ipAddr = ipAddr;
}

Client::~Client() {}

void Client::setMessage(const std::string &msg)
{
    std::string tmp(this->message + msg);
    this->message = tmp;
}

void Client::appendMessage(char &c)
{
    this->message += c;
}

void Client::clearBuffer()
{
    this->message = "";
}

std::string Client::getMessage()
{
    return this->message;
}

bool Client::isAuthentified()
{
    return Authentified;
}

bool Client::isValidPAss()
{
    return ValidPAss;
}

void Client::setPassFlagTrue()
{
    this->ValidPAss = true;
}

void Client::setPassFlagFalse()
{
    this->ValidPAss = false;
}

void Client::setNickFlagTrue()
{
    this->ValidNick = true;
}

void Client::setNickFlagFalse()
{
    this->ValidNick = false;
}

void Client::setUserFlagTrue()
{
    this->ValidUser = true;
}

void Client::setUserFlagFalse()
{
    this->ValidUser = false;
}

void Client::setClientPass(const std::string &pass)
{
    this->password = pass;
}

void Client::setClientNick(const std::string &nick)
{
    this->nick = nick;
}

void Client::setUserName(const std::string &username)
{
    this->userName = username;
}

void Client::setHostName(std::string &hostname)
{
    this->hostName = hostname;
}
void Client::setServName(std::string &servname)
{
    this->serverName = servname;
}
void Client::setRealName(std::string &realname)
{
    this->realName = realname;
}

std::string & Client::getNickName()
{
    return nick;
}

bool Client::authDone()
{
    if (ValidNick && ValidPAss && ValidUser)
        return true;
    return false;
}

int Client::getFlag() const
{
    return this->flag;
}

void Client::setFlag(int flag)
{
    this->flag = flag;
}

std::string & Client::getPassword()
{
    return this->password;
}
std::string   &  Client::getUserName()
{
    return (userName);
}

void Client::setAuthFlag()
{
    this->Authentified = true;
}

int Client::getFd() const
{
    return this->socket_fd;
}

void Client::set_clt_chans(const std::string & ch_name, int role)
{
    clt_chans.insert(std::make_pair(ch_name, role));
}

std::map<std::string, int> & Client::getClientChannels()
{
    return clt_chans;
}

std::string Client::getIp() const
{
    return this->ipAddr;
}

int Client::getMembership(const std::string   & chanName)
{
    return (clt_chans[chanName]);
}

void    Client::EditMembership(const std::string & chanName, int Membership)
{
    clt_chans[chanName] = Membership;
}

void   Client::EditNick(std::string oldNick, std::string newNick)
{
    std::map<std::string, int>::iterator it = clt_chans.begin();
    for (; it != clt_chans.end(); it++) {
        find_channel((*it).first).set_new_nick(oldNick, newNick);
    }
}
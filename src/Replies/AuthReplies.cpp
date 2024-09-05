#include "../../inc/Replies.hpp"


void Replies::welcomeMsg(Client & client)
{
    send_reply(SERVER_NAME " 001 " + client.getNickName() + " :Welcome to the olabrahm.akasex.chat Internet Relay Network " + client.getNickName(), client);
    send_reply(SERVER_NAME  " 002 " + client.getNickName() + " :Your host is olabrahm.akasex.chat, running version i1", client);
    send_reply(SERVER_NAME  " 003 " + client.getNickName() + " :This server was created Tue JUL 01 2024 at 21:13:03 UTC", client);
}

void Replies::unknownCmdMsg(const std::string & cmd, Client & client)
{
    if (cmd != "PONG")
        send_reply(SERVER_NAME " 421 " + client.getNickName() + " " + cmd + " :Unknown command" , client);
}

void Replies::mayNotReregisterMsg(Client & client)
{
    send_reply(SERVER_NAME " 462 " + client.getNickName() + " :You may not reregister" , client);
}

void Replies::notRegistredMsg(Client & client)
{
    send_reply(SERVER_NAME " 451 " + client.getNickName() + " :you have not registred" , client);
}

void Replies::incorectPassMsg(Client & client)
{
    send_reply(SERVER_NAME " 464 " + client.getNickName()+ " :Password incorrect" , client);
}

void Replies::AlreadyInUseMsg(const std::string & nick, Client & client)
{
    send_reply(SERVER_NAME " 433 " + client.getNickName() + " " + nick + " :Nickname is already in use" , client);
}

void Replies::NoNickGivenMsg(Client & client)
{
    send_reply(SERVER_NAME " 431 " + client.getNickName() + " :No nickname given" , client);
}

void Replies::ErroneusNickMsg(const std::string & nick, Client & client)
{
    send_reply(SERVER_NAME " 432 " + nick + " :Erroneus nickname" , client);
}
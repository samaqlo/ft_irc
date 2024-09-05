#pragma once
#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>

class Client;
class Channel;

class Replies {
    public:
        //Authentication replies
        static void welcomeMsg(Client & client);
        static void unknownCmdMsg(const std::string & cmd, Client & client);
        static void mayNotReregisterMsg(Client & client);
        static void notRegistredMsg(Client & client);
        static void incorectPassMsg(Client & client);
        static void AlreadyInUseMsg(const std::string & nick, Client & client);
        static void NoNickGivenMsg(Client & client);
        static void ErroneusNickMsg(const std::string & nick, Client & client);
        //channels replies
        static void RPL_CHANNELMODEIS(const std::string & channel, Client & client);
        static void ERR_CHANOPRIVSNEEDED(std::string name, Client & client);
        static void ERR_NOSUCHNICK(std::string & nick, Client & client);
        static void ERR_NOTONCHANNEL(const std::string & name ,Client & client);
        static void ERR_UNKNOWNMODE(std::string & mode , Client & client);
        static void ERR_NOTONCHANNEL(std::string & name ,Client & client);
        // static void ERR_NEEDMOREPARAMS(Client & client);
        static void ERR_UNKNOWNMODE(char mode , Client & client);
        static void ERR_KEYALREADYSET(const std::string & chan,Client & client);
        //join replies
        static void RPL_TOPIC(const Channel & chan, Client & client);
        static void RPL_TOPICWHOTIME(const Channel & chan, Client & client);
        static void RPL_NAMREPLY(Channel & chan, Client & client);
        static void RPL_ENDOFNAMES(const Channel & chan, Client & client);
        static void ERR_INVITEONLYCHAN(const std::string & name, Client & client);
        static void ERR_BADCHANNELKEY(const std::string & name, Client & client);
        static void ERR_NEEDMOREPARAMS(const std::string & cmd, Client & client);
        static void ERR_BADCHANMASK(const std::string & cmd, Client & client);
        static void ERR_CANNOTSENDTOCHAN(const std::string & channel, Client & client);
        static void ERR_NORECIPIENT(const std::string & cmd, Client & client);
        static void ERR_NOTEXTTOSEND(Client & client);
        static void ERR_NOSUCHCHANNEL(const std::string & channel, Client & client);
        static void RPL_NOTOPIC(const std::string & channel, Client & client);
        // static void ERR_USERNOTINCHANNEL(const std::string & target, const std::string & channel, Client & client);
        static void ERR_USERONCHANNEL(const std::string & target, const std::string & channel, Client & client);
        static void RPL_INVITING(const std::string & target, const std::string & channel, Client & client);
        static void ERR_NOSUCHCHANNEL(std::string & chan,Client & client);
        static void ERR_USERNOTINCHANNEL(const std::string & chan,const std::string & nick, Client & client);
        static void ERR_CHANNELISFULL(const std::string & chan, Client & client);
        
};
#include "../../inc/Replies.hpp"

void Replies::RPL_CHANNELMODEIS(const std::string & channel, Client & client)
{
    Channel & chan = find_channel(channel);
    std::string modes("+");
    if (chan.check_mode(INVITE_ONLY))
        modes.push_back('i');
    if (chan.check_mode(PASSWORD))
        modes.push_back('k');
    if (chan.check_mode(ISTOPIC))
        modes.push_back('t');
    if (chan.check_mode(LIMIT))
        modes.push_back('l');
    send_reply(SERVER_NAME " 324 " + client.getNickName() + " " + channel + " " + modes, client);
}

void Replies::ERR_CHANOPRIVSNEEDED(std::string channel, Client & client)
{
    send_reply(SERVER_NAME " 482 " + client.getNickName() + " " + channel + " :You're not channel operator", client);
}

void Replies::ERR_NOSUCHNICK(std::string & nick, Client & client)
{
    send_reply(SERVER_NAME " 401 " + client.getNickName() + " " + nick + " :No such nick/channel", client);
}

void Replies::ERR_NOTONCHANNEL(const std::string & channel ,Client & client)
{
    send_reply(SERVER_NAME " 442 " + client.getNickName() + " " + channel + " :You're not on that channel", client);
}

void Replies::ERR_UNKNOWNMODE(char  mode , Client & client)
{
    std::string unknownmode;
    unknownmode.push_back(mode);
    send_reply(SERVER_NAME " 472 " + client.getNickName() + " " + unknownmode + " :is unknown mode char to me", client);
}

void Replies::RPL_TOPIC(const Channel & chan, Client & client)
{
    send_reply(SERVER_NAME " 332 " + client.getNickName() + " " + chan.get_Name() + " : " + chan.getTopicContent(), client);
}

void Replies::RPL_TOPICWHOTIME(const Channel & chan, Client & client)
{
    send_reply(SERVER_NAME " 333 " + client.getNickName() + " " + chan.get_Name() + " " + chan.getTopicCreator() + " "  + _to_string(chan.getTopicTime()), client);
}

void Replies::RPL_NAMREPLY(Channel & chan, Client & client)
{
    send_reply(SERVER_NAME " 353 " + client.getNickName() + " = " + chan.get_Name() + " :" + chan.printMembers(), client);
}

void Replies::RPL_ENDOFNAMES(const Channel & chan, Client & client)
{
    send_reply(SERVER_NAME " 366 " + client.getNickName() + " " + chan.get_Name() + " :End of /NAMES list.", client);
}

void Replies::ERR_INVITEONLYCHAN(const std::string & name, Client & client)
{
    send_reply(SERVER_NAME " 473 " + name + " :Cannot join channel (+i)", client);
}

void Replies::ERR_BADCHANNELKEY(const std::string & name, Client & client)
{
    send_reply(SERVER_NAME " 475 " + name + " :Cannot join channel, you need the correct key (+k)", client);
}

void Replies::ERR_NEEDMOREPARAMS(const std::string & cmd, Client & client)
{
    send_reply(SERVER_NAME " 461 " + cmd + " :Not enough parameters", client);
}

void Replies::ERR_BADCHANMASK(const std::string & name, Client & client)
{
    send_reply(SERVER_NAME " 476 " + name + " :Bad Channel Mask", client);
}

void Replies::ERR_CANNOTSENDTOCHAN(const std::string & name, Client & client)
{
    send_reply(SERVER_NAME " 404 " + client.getNickName() + " " + name + " :Cannot send to channel", client);
}

void Replies::ERR_NORECIPIENT(const std::string & cmd, Client & client)
{
    send_reply(SERVER_NAME " 411 " + client.getNickName() + " :No recipient given (" + cmd + ")", client);
}

void Replies::ERR_NOTEXTTOSEND(Client & client)
{
    send_reply(SERVER_NAME " 412 " + client.getNickName() + " :No text to send", client);
}

void Replies::ERR_NOSUCHCHANNEL(const std::string & channel, Client & client)
{
    send_reply(SERVER_NAME " 403 " + client.getNickName() + " " + channel + " :No such channel", client);
}

void Replies::RPL_NOTOPIC(const std::string & channel, Client & client)
{
    send_reply(SERVER_NAME " 331 " + client.getNickName() + " " + channel + " :No topic is set", client);
}

// void Replies::ERR_USERNOTINCHANNEL(const std::string & target, const std::string & channel, Client & client)
// {
//     send_reply(SERVER_NAME " 441 " + client.getNickName() + " " + target + " " + channel + " :They aren't on that channel", client);
// }

void Replies::ERR_USERONCHANNEL(const std::string & target, const std::string & channel, Client & client)
{
    send_reply(SERVER_NAME " 443 " + client.getNickName() + " " + target + " " + channel + " :is already on channel", client);
}

void Replies::RPL_INVITING(const std::string & target, const std::string & channel, Client & client)
{
    send_reply(SERVER_NAME " 341 " + client.getNickName() + " " + target + " " + channel, client);
}

void Replies::ERR_NOSUCHCHANNEL(std::string & chan,Client & client)
{
    send_reply(SERVER_NAME " 403 " + client.getNickName() + " " + chan + " :No such channel", client);
}

void Replies::ERR_KEYALREADYSET(const std::string & chan,Client & client)
{
    send_reply(SERVER_NAME " 467 " + client.getNickName() + " " + chan + " :Channel key already set", client);
}

void Replies::ERR_USERNOTINCHANNEL(const std::string & chan,const std::string & nick, Client & client)
{
    send_reply(SERVER_NAME " 441 " + client.getNickName() + " " + nick + " " + chan + " :They aren't on that channel", client);
}

void  Replies::ERR_CHANNELISFULL(const std::string & chan, Client & client)
{
    send_reply(SERVER_NAME " 471 " + client.getNickName() + " " + chan + " :Cannot join channel, Channel is full (+l)", client);
}
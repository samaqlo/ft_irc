 #pragma once

#include <ctime>
#include <iostream>
#include "Client.hpp"
#include <map>

    enum unset_mode
    {
        UINVITEONLY,
        UTOPIC,
        UPASSWORD,
        ULIMIT
    };
    enum MODE
    {
        NO_MODE,
        INVITE_ONLY = 1,
        ISTOPIC = 2,
        PASSWORD = 4,
        LIMIT = 8
    };


    enum MEMBERSHIP
    {
        OP = 0,
        REG = 1,
    };


class Client;

class Channel
{
    private:
        std::string     channel_name;
        std::map<std::string,std::pair<int, Client> > members;
        std::vector<std::string>    inviteList;
        std::string     key;
        std::string     topicCreator;
        std::string     topicContent;
        time_t          topicTime;
        int             mode;
        int             limit;
    public:
        Channel();
        Channel(std::string chan, std::string key);
        // int8_t  is_topic;
        // int     mode;
        // SETTERS
        void    setTopic(const std::string & nick, const std::string & content, time_t time);
        void    set_member(Client & clt, int role);
        void    set_name(const std::string & name);
        void    set_key(const std::string & key);
        void    set_mode(int mode);
        void    setInviteList(const std::string & nick);
        void    set_limit(int lim);
        void    unset_mode(int mode);
 
        // GETTERS
        std::string getTopicCreator() const;
        std::string getTopicContent() const;
        time_t      getTopicTime() const;
        std::map<std::string,std::pair<int, Client> > & getMembers();
        const std::string & get_Name() const;
        const std::string & get_Key() const;
        int get_Mode() const;
        int get_limit();
        int check_mode(int to_check);
        std::vector<std::string> & getInviteList();


        //functions
        void    broadcast(std::string  msg, bool op);
        std::string    printMembers();
        bool    is_member(Client & clt);
        bool    is_client(const std::string & nick);
        bool    is_operator(Client & clt);
        void    del_member(const std::string & nick);
        int get_num_members();
        void    set_operator(std::string & nick, int Membership);
        bool    is_invited(std::string nick);
        void    set_new_nick(std::string oldNick, std::string  newNick);
};
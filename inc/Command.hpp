#pragma once 

#include "Base.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include <vector>
#include <cstring>

enum modes
{
        key = 1,
        invite = 2,
        topic = 3,
        opera = 4,
        lim = 5
};
    
enum e_commands
{
    NOT_FOUND,
    PASS,
    USER,
    NICK,
    JOIN,
    MODE,
    KICK,
    INVITE,
    TOPIC,
    PRIVMSG,
    BOT,
};

class Command
{
    private:
        std::vector<std::string> _cmd;
        Client & clt;
        int cmd_id;
    public:
        static const char *urls[];
        
        Command(const std::vector<std::string>  & cmd, Client & clt);

        void    identify_cmd();
        void    invoke_cmd();

        void    join();
        void    bot();
        void    mode();
        void    topic();
        void    kick();
        void    invite();
        void    privmsg();

        void bot_answer(std::string msg);
        void bot_answer_to_random(std::string msg);
};

//JOIN utils
int validate_name(const std::string & name);
std::map<std::string, std::string>  parse_args(std::vector<std::string> args, Client & clt);
std::vector<std::string>  split(std::string & cmd, char del);
int is_exist(const std::string & name);
void    create_channel(std::map<std::string,std::string>::iterator it, Client & clt);
bool    join_channel(std::map<std::string,std::string>::iterator it, Client & clt);
Channel & find_channel(const std::string & name);
void    send_reply(std::string  msg, Client  clt);
int channel_index(const std::string & name);
//Client_Channel utils
bool    is_channel(std::string & channel);
Client & find_client(const std::string & nick);
bool    is_client(std::string & nick);
std::string  _to_string(long long num);
bool    check_modestring(std::string & str);
int    distributeMsg(std::pair<std::string, bool> & target, std::string & text, Client &clt);
bool    check_modestring(std::vector<std::string> args, Client & clt);

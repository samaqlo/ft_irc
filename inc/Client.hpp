#pragma once 

# include "Base.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include <string>


class Client
{
    private:
        std::string nick;
        std::string message;
        std::string userName;
        std::string hostName;
        std::string serverName;
        std::string realName;
        std::string password;
        bool Authentified;
        bool ValidPAss;
        bool ValidNick;
        bool ValidUser;
        int socket_fd;
        std::string ipAddr;
        std::string hostname;
        int flag;
        std::map<std::string, int> clt_chans;

    public:
        Client();
        Client(int clientSocket, std::string ipAddr);
        ~Client();

        void clearBuffer();
        bool isAuthentified();
        bool isValidPAss();
        bool authDone();

        //GETERS//
        std::string getMessage();
        std::string & getNickName();
        std::string & getUserName();
        std::string & getPassword();
        int getFlag() const;
        int getFd() const;
        std::string getIp() const;
        std::map<std::string, int> & getClientChannels();
        int getMembership(const std::string   & chanName);
    
        //SETERS//
        void setMessage(const std::string &msg);
        void appendMessage(char & c);
        void setAuthFlag();
        void setPassFlagTrue();
        void setPassFlagFalse();
        void setNickFlagTrue();
        void setNickFlagFalse();
        void setUserFlagTrue();
        void setUserFlagFalse();
        void setClientPass(const std::string &pass);
        void setClientNick(const std::string &nick);
        void setUserName(const std::string &username);
        void setHostName(std::string &hostname);
        void setServName(std::string &servname);
        void setRealName(std::string &realname);
        void setFlag(int flag);
        void set_clt_chans(const std::string & ch_name, int role);
        void setIp(char * ipAddr);

        //functs
        void   EditMembership(const std::string & chanName, int Membership);
        void   EditNick(std::string oldNick, std::string newNick);
};
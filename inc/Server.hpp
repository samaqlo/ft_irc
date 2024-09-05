#ifndef SERVER_H
# define SERVER_H

# include "Base.hpp"
# include "Client.hpp"
#include "TcpSocket.hpp"
#include "Channel.hpp"
#include <ctime>
#include <vector>
# include "Replies.hpp"

class Client;
class TcpSocket;
class Server
{
    private:
        std::string port;
        std::string password;
        std::vector<std::string> cmd;
        std::vector<std::string> nicknames;
        std::vector<struct pollfd> fds;
        TcpSocket serverSock;
        bool isValidCmd;
        int flag;
    public:
        Server();
        Server(const std::string &port, const std::string &password);
        ~Server();

        static std::vector<Client> clients;
        static std::vector<Channel> server_channels;
        static clock_t time_req;
        void parseArgs();
        void serverSetup();
        void startAccepting();
        void acceptClient(struct pollfd &fd);
        void receiveMsg(int idx);
        void handleCommand(const std::string &command, int clientIdx);
        void authentification(int &clientIdx);
        void authentificationHelper(int &clientIdx);
        bool parseCommand(const std::string &command, int clientIdx);
        bool commandValidation(int &clientIdx);
        void pass(int &clientIdx);
        void nick(int &clientIdx);
        bool parseNickName(std::string &nick, int &clientIdx);
        bool nickAlreadyExist(std::string &nick, int &clientIdx);
        void user(int &clientIdx);
        bool checkPassCorrect(std::string ServerPAss, std::string CLientPass);
        static std::string get_requested_time();

        //GETERS//
        std::string getPort();
        std::string getPasswd();

        //functs
        static void    quit_channels(Client & clt, bool flag);
        static std::vector<Channel>::iterator    erase_channel(const std::string & name);
        void    ChannelMonitoring();
};

#endif
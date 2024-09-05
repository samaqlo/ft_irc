# include "../../inc/Command.hpp"
#include <stdexcept>

void Command::bot_answer(std::string msg)
{
        std::string message = SERVER_NAME " 69 BOT :" + msg + "\r\n";
        if (send(clt.getFd(), message.c_str(), message.size(), 0) == -1)
            return ;
}

void Command::bot_answer_to_random(std::string msg)
{
        std::string message = SERVER_NAME " 69 BOT :" + msg + "\r\n";
        int random_user = Server::clients[rand() % Server::clients.size()].getFd();
        if (random_user != clt.getFd())
            if (send(random_user, message.c_str(), message.size(), 0) == -1)
                return ;
}

void    Command::bot()
{
    if (_cmd.size() == 1)
        bot_answer("Type /help for options");
    else
    {
        if (_cmd[1] == "/users") {
            std::string clients_size;
            std::stringstream ss;
            ss << Server::clients.size();
            ss >> clients_size;
            bot_answer(clients_size + (clients_size == "1" ? " user" : " users"));
            ss.clear();
        }
        else if (_cmd[1] == "/infos") {
            bot_answer("Server Information :");
            bot_answer("- Server Address   : olabrahm.akasex.chat");
            bot_answer("- Server Creators  : obmium, 3dnan, Samaqlo");
            bot_answer("- Server Uptime    : " + Server::get_requested_time() + " seconds");
            bot_answer("Enjoy Your Stay.");
        }
        else if (_cmd[1] == "/music") {
            bot_answer(std::string("https://www.youtube.com/watch?v=") + urls[rand() % 5]);
        }
        else if (_cmd[1] == "/usmell") {
            bot_answer_to_random("You need to take a bath");
        }
        else if (_cmd[1] == "/help") {
            bot_answer("OPTIONS: /users, /infos, /music, /usmell");
        }
        else
            bot_answer("Invalid option");
    }
}
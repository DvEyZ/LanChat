#include "CLI.h"

CLI::CLI(std::vector <std::string> args)
    :silent_flag(false)
{
    // set up used commands
    commands.emplace(Command("con", 1, 
        [this] (std::vector <std::string> args)
        {
            app->connect(args[0]);
        }
    ));

    commands.emplace(Command("discon", 0, 
        [this] (std::vector <std::string> args)
        {
            app->disconnect();
        }
    ));

    commands.emplace(Command("id", 0,
        [this] (std::vector <std::string> args)
        {
            silent();
            auto name = ask("Username: ");
            auto pass = ask("Password: ");
            nosilent();
            app->identify(name, pass);
        }
    ));

    commands.emplace(Command("id", 1,
        [this] (std::vector <std::string> args)
        {
            silent();
            auto pass = ask("Password: ");
            nosilent();
            app->identify(args[0], pass);
        }
    ));

    commands.emplace(Command("id", 2,
        [this] (std::vector <std::string> args)
        {
            app->identify(args[0], args[1]);
        }
    ));

    commands.emplace(Command("m", 0,
        [this] (std::vector <std::string> args)
        {
            silent();
            auto to = ask("To: ");
            auto m = ask("Message: ");

            std::vector <std::string> temp;
            while(to.find(',') != std::string::npos)
            {
                temp.push_back(to.substr(0, to.find(',')));
                to.erase(0, to.find(','));
            }
            nosilent();
            app->sendUnicastMessage(temp, m);
        }   
    ));

    commands.emplace(Command("m", 1,
        [this] (std::vector <std::string> args)
        {
            std::vector <std::string> temp;
            while(args[0].find(',') != std::string::npos)
            {
                temp.push_back(args[0].substr(0, args[0].find(',')));
                args[0].erase(0, args[0].find(','));
            }
            silent();
            auto m = ask("Message: ");
            nosilent();
            app->sendUnicastMessage(temp, m);
        }
    ));

    commands.emplace(Command("@", 0,
        [this] (std::vector <std::string> args)
        {
            silent();
            auto m = ask("Message: ");
            nosilent();
            app->sendBroadcastMessage(m);
        }
    ));
    
    commands.emplace(Command("s", 1,
        [this] (std::vector <std::string> args)
        {
            if(args[0] == "on")
                silent();
            else if(args[0] == "off")
                nosilent();
            else
                app->error("Invalid arguments.");
        }
    ));

    commands.emplace(Command("help", 0,
        [this] (std::vector <std::string> args)
        {
            silent();
            app->help();
            nosilent();
        }
    ));

    commands.emplace(Command("quit", 0,
        [this] (std::vector <std::string> args)
        {
            app->exit(0);
        }
    ));
}

void CLI::run()
{
    joyterm::init();
    while(true)
    {
        std::string cmd = readCommand();
        executeCommand(cmd);
    }
}

std::string CLI::readCommand()
{
    std::string str;
    std::getline(std::cin, str);
    return str;
}

void CLI::silent()
{
    silent_flag = true;
}

void CLI::nosilent()
{
    silent_flag = false;
    std::cout << os.str();
}
void CLI::writeMessage(std::string sender, std::vector <std::string> receivers, std::string message)
{
    std::string temp;

    temp += "\n[ " + sender + " ] => ";
    
    for(auto i : receivers)
    {
        temp += i + ",";
    }
    temp.erase(temp.end() - 1);

    temp += "\n" + message;


    write(temp + "\n");
}

void CLI::writeError(std::string message)
{
    std::ostringstream strm; 
    strm << joyterm::style::MStyle({joyterm::style::FgColor::red}) << 
        "ERROR: " << message 
    << joyterm::style::MStyle({joyterm::style::Style::reset}) << "\n";
    
    write(strm.str());
}

void CLI::writeInfo(std::string message)
{
    write(message + "\n");
}

void CLI::write(std::string message)
{
    cout_mutex.lock();
    (silent_flag ? os : std::cout) << message;
    cout_mutex.unlock();
}

void CLI::executeCommand(std::string command)
{
    std::string operation = command.substr(0, command.find(' '));
    command.erase(0, command.find(' ') + 1);
    
    std::vector <std::string> args;

    while(command.find(' ') != std::string::npos)
    {
        args.push_back(command.substr(0, command.find(' ')));
        command.erase(0, command.find(' ') + 1);    
    }

    auto op = commands.find(Command(operation, args.size()));

    if(op == commands.end())
    {
        app->error("Command not found.");
        return;
    }

    op->run(args);
}
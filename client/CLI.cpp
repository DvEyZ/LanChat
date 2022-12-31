#include "CLI.h"

CLI::CLI(std::vector <std::string> args)
    :silent_flag(false), running(true)
{
    // set up used commands
    commands.push_back(Command("connect", 1, 
        [this] (std::vector <std::string> args)
        {
            app->connect(args[0]);
        }
    ));

    commands.push_back(Command("disconnect", 0, 
        [this] (std::vector <std::string> args)
        {
            app->disconnect();
        }
    ));

    commands.push_back(Command("identify", 0,
        [this] (std::vector <std::string> args)
        {
            auto name = ask("Username: ");
            auto pass = ask("Password: ");
            app->identify(name, pass);
        }
    ));

    commands.push_back(Command("identify", 1,
        [this] (std::vector <std::string> args)
        {
            auto pass = ask("Password: ");
            app->identify(args[0], pass);
        }
    ));

    commands.push_back(Command("identify", 2,
        [this] (std::vector <std::string> args)
        {
            app->identify(args[0], args[1]);
        }
    ));

    commands.push_back(Command("message", 0,
        [this] (std::vector <std::string> args)
        {
            auto to = ask("To: ");
            auto m = ask("Message: ");

            std::vector <std::string> temp;
            while(to.find(',') != std::string::npos)
            {
                temp.push_back(to.substr(0, to.find(',')));
                to.erase(0, to.find(','));
            }
            app->sendUnicastMessage(temp, m);
        }   
    ));

    commands.push_back(Command("message", 1,
        [this] (std::vector <std::string> args)
        {
            std::vector <std::string> temp;
            while(args[0].find(',') != std::string::npos)
            {
                temp.push_back(args[0].substr(0, args[0].find(',')));
                args[0].erase(0, args[0].find(','));
            }
            auto m = ask("Message: ");
            app->sendUnicastMessage(temp, m);
        }
    ));

    commands.push_back(Command("@", 0,
        [this] (std::vector <std::string> args)
        {
            auto m = ask("Message: ");
            app->sendBroadcastMessage(m);
        }
    ));
    
    commands.push_back(Command("silent", 1,
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

    commands.push_back(Command("help", 0,
        [this] (std::vector <std::string> args)
        {
            app->help();
        }
    ));

    commands.push_back(Command("quit", 0,
        [this] (std::vector <std::string> args)
        {
            app->exit();
        }
    ));
}

void CLI::run()
{
    joyterm::init();
    while(running)
    {
        std::string cmd = readCommand();
        executeCommand(cmd);
    }
}

std::string CLI::readCommand()
{
    std::string str;
    write("> ");
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

std::string CLI::ask(std::string what)
{
    std::string answer;
    silent();
    write(what);
    std::getline(std::cin, answer);
    nosilent();
    return answer;
}

void CLI::writeMessage(std::string sender, std::vector <std::string> receivers, std::string message)
{
    using namespace joyterm::style;
    std::ostringstream strm;

    strm << "\n" << MStyle({FgColor::blue}) << "[ " << MStyle({Style::reset})
        << sender 
        << MStyle({FgColor::blue}) << " ]" << MStyle({Style::reset}) << " => ";
    
    for(auto i : receivers)
    {
        strm << i << MStyle({FgColor::yellow}) << "," << MStyle({Style::reset});
    }

    strm << "\n" << message << "\n";


    write(strm.str());
}

void CLI::writeError(std::string message)
{
    using namespace joyterm::style;
    std::ostringstream strm; 
    strm << MStyle({FgColor::red}) << 
        "ERROR: " << message 
    << MStyle({Style::reset}) << "\n";
    
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

    try
    {
        Command cmd = findUniqueCommand(operation, args.size());
        cmd.run(args);
    }
    catch(const int err)
    {
        if(err == 0)
            app->error("Command not found.");
        else
            app->error("Ambiguous command.");
        return;
    }
}

Command CLI::findUniqueCommand(std::string name, int args)
{
    std::vector <Command> matches;
    std::copy_if(commands.begin(), commands.end(), std::back_inserter(matches), 
        [name, args] (Command command)
        {
            // if command name begins with provided name, and argument number matches, return true, else false.
            if(command.name.find(name) == 0)
            {
                if(command.arg_num == args)
                {
                    return true;
                }
            }
            return false;
        }
    );

    // check if the command name is unique

    if(matches.size() == 1) return matches[0];
    else if(matches.size() == 0) throw 0;
    else throw 1;
}

void CLI::stop()
{
    running = false;
}
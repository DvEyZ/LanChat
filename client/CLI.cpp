#include "CLI.h"

CLI::CLI(App* _app, std::vector <std::string> args)
    :app(_app), silent_flag(false), running(true)
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
            std::istringstream strm(to);
            std::string tmp;

            while(std::getline(strm, tmp, ','))
            {
                temp.push_back(tmp);
            }
            // !!!

            app->sendUnicastMessage(temp, m);
        }   
    ));

    commands.push_back(Command("message", 1,
        [this] (std::vector <std::string> args)
        {
            std::vector <std::string> temp;
            std::istringstream strm(args[0]);
            std::string tmp;

            while(std::getline(strm, tmp, ','))
            {
                temp.push_back(tmp);
            }
            // !!!
            
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
    write(what);
    std::getline(std::cin, answer);
    return answer;
}

void CLI::writeMessage(std::string sender, std::vector <std::string> receivers, std::string message)
{
    using namespace joyterm::style;
    std::ostringstream strm;

    strm << "\n" << MStyle({FgColor::b_blue}) << "[ " << MStyle({Style::reset})
        << sender 
        << MStyle({FgColor::b_blue}) << " ]" << MStyle({Style::reset}) << " => ";
    
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
    strm << MStyle({FgColor::b_red, Style::bold}) << 
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
    (silent_flag ? os : std::cout) << std::flush;
    (silent_flag ? os : std::cout) << message;
    cout_mutex.unlock();
}

void CLI::executeCommand(std::string command)
{
    if(command == "") return; // skip empty commands in order not to make output mess

    auto f = command.find(' ');
    std::string operation = command.substr(0, f);
    if(f != std::string::npos)
        command.erase(0, f);
    else
        command.erase(command.begin(), command.end());
    
    std::vector <std::string> args;

    if(f != std::string::npos)
    {
        std::istringstream strm(command);
        std::string temp;

        while(std::getline(strm, temp, ' '));
        {
            args.push_back(temp);
        }
    }

    // todo: delete all args that are whitespace-only.
    
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
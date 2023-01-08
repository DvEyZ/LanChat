//TODO:
//Implement BasicAuth()

#include "BasicAuth.h"

BasicAuth::BasicAuth(std::shared_ptr <Logger> _logger)
{
	logger = _logger;

	useDefaultConfig();

	std::ifstream config_file(AUTH_CONFIG_FILE_SERVER);
	nlohmann::json config;
	
	try
	{
		config = nlohmann::json::parse(config_file);
	}
	catch(nlohmann::json::exception)
	{
		logger->log("Something went wrong with configuration file.");
		useDefaultConfig();
		return;
	}

	try	{	require_account = config["auth"]["requireAccount"].get<bool>();								} catch(nlohmann::json::exception) {}
	
	//optional values
	try {	require_password = config["auth"]["requirePassword"].get<bool>();							} catch(nlohmann::json::exception) {}
	
	try {	max_connections_from_ip = config["connectionPolicy"]["maxConnectionsFromIp"].get<int>();	} catch(nlohmann::json::exception) {}
	try {	max_connections_for_user = config["connectionPolicy"]["maxConnectionsForUser"].get<int>();	} catch(nlohmann::json::exception) {}
	try {	banned_ips = config["connectionPolicy"]["bannedIps"].get<std::set<std::string>>();			} catch(nlohmann::json::exception) {}

	try {	anyone_may_broadcast = config["messagePolicy"]["anyoneMayBroadcast"].get<bool>();			} catch(nlohmann::json::exception) {}
	try {	banned_words = config["messagePolicy"]["bannedWords"].get<std::set<std::string>>();			} catch(nlohmann::json::exception) {}
	try {	broadcast_users = config["messagePolicy"]["broadcastUsers"].get<std::set<std::string>>();	} catch(nlohmann::json::exception) {}

	if(require_account)
	{
		getUserList();
	}
}

void BasicAuth::useDefaultConfig()
{
	require_account = false;
	banned_ips = {};
	anyone_may_broadcast = true;
	banned_words = {};
	broadcast_users = {};
}

void BasicAuth::getUserList()
{
	std::ifstream user_file(USER_LIST_FILE);
	nlohmann::json users;

	try 
	{
		users = nlohmann::json::parse(user_file);
		user_list = users["users"].get<std::map<std::string, std::string>>();
	}
	catch(nlohmann::json::exception)
	{
		logger->log("Something went wrong with user file, using default settings.");
	}
}

ResponseMessage BasicAuth::authenticate(IdentifyMessage message)
{
    bool accepted = false;
	if(!require_account)	
        accepted = true;
	else if(checkPassword(message.getUsername(), message.getPassword()))
		accepted = true;

    if(accepted)
        return ResponseMessage({ "Welcome." }, ResponseMessage::DONE);
    else
        return ResponseMessage({ "Login failed." }, ResponseMessage::DENIED);
}

ResponseMessage BasicAuth::permitConnection(std::shared_ptr <Session> session)
{
	if(chat == nullptr)
	{
		logger->log("ERROR: chat is null.");
		return ResponseMessage({ "Unspecified error." }, ResponseMessage::FAIL);
	}
	try
	{
	if(chat->getUserConnections(session->getUser()).size() >= max_connections_for_user.value())	
		return ResponseMessage({ "You have too many sessions opened." }, ResponseMessage::DENIED);
	}
	catch(std::bad_optional_access)
	{}
	try
	{
	if(chat->getIpConnections(session->getConnection()->getRemoteIp()).size() >= max_connections_from_ip.value())	
		return ResponseMessage({ "Too many connections from your IP." }, ResponseMessage::DENIED);
	}
	catch(std::bad_optional_access)
	{}
	if(banned_ips.contains(session->getConnection()->getRemoteIp()))
		return ResponseMessage({ "Your IP is on the server blacklist." }, ResponseMessage::DENIED);
	return ResponseMessage({"Connection accepted."}, ResponseMessage::DONE);
}

bool BasicAuth::permitMessage(Message& message)
{
    // TODO

	return true;
}

bool BasicAuth::checkPassword(std::string user, std::string password)
{
	if(!require_password) return true;
	try
	{
		if(user_list.at(user) == password)	return true;
	}
	catch(std::out_of_range)
	{
		return false;
	}
	return false;
}

/*
bool BasicAuth::containsBannedWords(std::string message)
{
	for(auto i : banned_words)
	{
		if(string_contains(message, i)) return true;
	}
	return false;
}
*/

bool BasicAuth::isAllowedToBroadcast(std::string user)
{
	if(anyone_may_broadcast) return true;
	if(broadcast_users.contains(user)) return true;
	else return false;
}
#pragma once

#define LOG_FILE_SERVER "LanChat.log"
#define CONFIG_FILE_SERVER "LanChat.conf.json"
#define AUTH_CONFIG_FILE_SERVER "Auth.conf.json"
#define USER_LIST_FILE "Users.conf.json"

#define MESSAGE_HEADER_LENGTH 4
#define MESSAGE_FIELD_SEPARATOR '\x01'

#define CONNECTION_MAX_MALFORMED_IN_A_ROW 3
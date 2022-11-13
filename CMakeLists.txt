cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_CXX_STANDARD 20)

set(ARCHIVE_OUTPUT_DIRECTORY "../lib")
set(LIBRARY_OUTPUT_DIRECTORY "../lib")
set(RUNTIME_OUTPUT_DIRECTORY "../bin")

project(LanChat)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	include_directories("C:\\Boost\\boost_1_80_0\\boost_1_80_0")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# include_directories()
endif()

add_custom_target(
	LibLanChatGeneric
	add_library(LibLanChatGeneric "ChatMessage.cpp" "IdentifyMessage.cpp" "IdentifyResponseMessage.cpp" "helper_functions.cpp")
	DEPENDS "ChatMessage.cpp" "IdentifyMessage.cpp" "IdentifyResponseMessage.cpp" "helper_functions.cpp"
)

add_custom_target(
	LibLanChatServer
	add_library(LibLanChatServer "server/Auth.cpp" "server/Chat.cpp" "server/Connection.cpp" "server/Server.cpp" "server/Logger.cpp")
	target_link_libraries(LibLanChatServer LibLanChatGeneric)
	DEPENDS "server/Auth.cpp" "server/Chat.cpp" "server/Connection.cpp" "server/Server.cpp" "server/Logger.cpp" LibLanChatGeneric
)

add_custom_target(
	LanChatServer
	add_executable(LanChatServer "server/BasicAuth.cpp" "server/BasicChat.cpp" "server/StdOstreamLogger.cpp" "server/main.cpp")
	target_link_libraries(LanChatServer LibLanChatServer)
	DEPENDS "server/BasicAuth.cpp" "server/BasicChat.cpp" "server/StdOstreamLogger.cpp" "server/main.cpp" LibLanChatServer
)
cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_CXX_STANDARD 20)

project(LanChat)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	include_directories("C:\\Boost\\boost_1_80_0\\boost_1_80_0")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# include_directories(linux includes)
endif()

add_library(LibLanChatGeneric Message.cpp ChatMessage.cpp IdentifyMessage.cpp IdentifyResponseMessage.cpp helper_functions.cpp)
add_library(LibLanChatServer server/Auth.cpp server/Connection.cpp server/Server.cpp)
target_link_libraries(LibLanChatServer LibLanChatGeneric)

add_executable(LanChatServer server/BasicAuth.cpp server/BasicChat.cpp server/StdOstreamLogger.cpp server/FileLogger.cpp server/main.cpp)
target_link_libraries(LanChatServer LibLanChatServer)

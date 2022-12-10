cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_CXX_STANDARD 20)

project(LanChat)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	include_directories("C:\\Boost\\boost_1_80_0\\boost_1_80_0" "C:\\Users\\kwiregowo5\\Desktop\\SZYMON\\projekty\\C++\\include")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# include_directories(linux includes)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	# MSVC directives
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_link_options(-pthread)
endif()

add_library(LibLanChatGeneric Message.cpp ChatMessage.cpp IdentifyMessage.cpp IdentifyResponseMessage.cpp helper_functions.cpp)
add_library(LibLanChatServer server/Auth.cpp server/SocketConnection.cpp server/Server.cpp server/Session.cpp server/ConnectionException.cpp)
target_link_libraries(LibLanChatServer LibLanChatGeneric)

add_executable(LanChatServer server/BasicAuth.cpp server/BasicChat.cpp server/StdOstreamLogger.cpp server/FileLogger.cpp server/LoggerComposite.cpp server/main.cpp)
target_link_libraries(LanChatServer LibLanChatServer)

enable_testing()

add_executable(ChatMessageTest tests/ChatMessageTest.cpp)
target_link_libraries(ChatMessageTest GTest::gtest_main)
target_link_libraries(ChatMessageTest LibLanChatGeneric)

add_executable(IdentifyMessageTest tests/IdentifyMessageTest.cpp)
target_link_libraries(IdentifyMessageTest GTest::gtest_main)
target_link_libraries(IdentifyMessageTest LibLanChatGeneric)

add_executable(IdentifyResponseMessageTest tests/IdentifyResponseMessageTest.cpp)
target_link_libraries(IdentifyResponseMessageTest GTest::gtest_main)
target_link_libraries(IdentifyResponseMessageTest LibLanChatGeneric)

add_executable(SessionTest tests/SessionTest/SessionTest.cpp tests/SessionTest/TestConnection.cpp)
target_link_libraries(SessionTest GTest::gtest_main)
target_link_libraries(SessionTest LibLanChatGeneric)

include(GoogleTest)

gtest_discover_tests(ChatMessageTest)
gtest_discover_tests(IdentifyMessageTest)

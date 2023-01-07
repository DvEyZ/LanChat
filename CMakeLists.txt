cmake_minimum_required(VERSION 3.9.1)
set(CMAKE_CXX_STANDARD 20)

project(LanChat)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	include(CMakeIncludes.txt)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	add_link_options(-pthread)
endif()

add_library(LibLanChatMessages
    "message/Message.cpp"
    "message/Addressed.cpp"
    "message/Readable.cpp"
    "message/Signed.cpp"
    "message/ReadableMessageBody.cpp"
    "message/SendMessage.cpp"
    "message/ResponseMessage.cpp"
    "message/IdentifyMessage.cpp"
    "MessageWrapper.cpp"
)
add_library(LibLanChatGeneric 
	Message.cpp
	ChatMessage.cpp 
	IdentifyMessage.cpp
	IdentifyResponseMessage.cpp 
	SocketConnection.cpp 
	ConnectionError.cpp 
	helper_functions.cpp
)
add_library(LibLanChatServer server/Auth.cpp server/Server.cpp server/Session.cpp)
target_link_libraries(LibLanChatServer LibLanChatGeneric)

add_executable(LanChatServer server/BasicAuth.cpp server/BasicChat.cpp server/StdOstreamLogger.cpp server/FileLogger.cpp server/LoggerComposite.cpp server/main.cpp)
target_link_libraries(LanChatServer LibLanChatServer)

add_executable(LanChatClient client/main.cpp client/Session.cpp client/App.cpp client/Network.cpp client/Session.cpp client/CLI.cpp client/Command.cpp)
target_link_libraries(LanChatClient LibLanChatGeneric)

enable_testing()

add_executable(SendMessageTest tests/SendMessageTest.cpp)
target_link_libraries(SendMessageTest LibLanChatMessages GTest::gtest_main)

add_executable(IdentifyMessageTest tests/IdentifyMessageTest.cpp)
target_link_libraries(IdentifyMessageTest LibLanChatMessages GTest::gtest_main)

add_executable(ResponseMessageTest tests/ResponseMessageTest.cpp)
target_link_libraries(ResponseMessageTest LibLanChatMessages GTest::gtest_main)

add_executable(MessageWrapperTest tests/MessageWrapperTest.cpp)
target_link_libraries(MessageWrapperTest LibLanChatMessages GTest::gtest_main)


include(GoogleTest)

gtest_discover_tests(SendMessageTest)
gtest_discover_tests(IdentifyMessageTest)
gtest_discover_tests(ResponseMessageTest)
gtest_discover_tests(MessageWrapperTest)
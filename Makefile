CC = "g++"

messages.o: ChatMessage.h ChatMessage.cpp IdentifyMessage.h IdentifyMessage.cpp IdentifyResponseMessage.h IdentifyMessage.cpp helper_functions.h helper_functions.cpp
	$(CC) ChatMessage.cpp IdentifyMessage.cpp IdentifyResponseMessage.cpp -o ../bin/messages.o

server.o: server/Server.h server/Server.cpp server/Connection.h server/Connection.cpp server/Chat.h server/Chat.cpp server/Auth.h server/Auth.cpp:
	$(CC) server/Server.cpp server/Connection.cpp server/Chat.cpp server/Auth.cpp ../bin/messages.o -o ../bin/server.o
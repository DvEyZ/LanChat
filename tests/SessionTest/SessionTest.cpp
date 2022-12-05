#include "../../server/Session.h"
#include "TestConnection.h"
#include "../../server/BasicAuth.h"
#include "../../server/BasicChat.h"
#include "../../server/LoggerProxy.h"

class SessionTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		dummy_network = DummyNetwork();
		logger_dummy = LoggerProxy("", {});
		chat_dummy = BasicChat(&auth_dummy, &logger_dummy);
		connection_dummy = TestConnection(&dummy_network);
		session_dummy_1 = Session(boost::shared_ptr<Connection>(&connection_dummy), &chat_dummy, &logger_dummy);
	}

	DummyNetwork dummy_network;
	TestConnection connection_dummy;
	Session session_dummy_1;
	Session session_dummy_2;
	BasicChat chat_dummy;
	BasicAuth auth_dummy;
	LoggerProxy logger_dummy;
};
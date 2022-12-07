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
		logger_dummy = LoggerProxy("", {});
		chat_dummy = BasicChat(&auth_dummy, &logger_dummy);
		connection_dummy = TestConnection(&test_connection_sstream);
		session_dummy = Session(std::shared_ptr<Connection>(&connection_dummy), &chat_dummy, &logger_dummy);
	}

	TestConnection connection_dummy;
	Session session_dummy;
	BasicChat chat_dummy;
	BasicAuth auth_dummy;
	LoggerProxy logger_dummy;
	std::stringstream test_connection_sstream;
};

TEST_F(SessionTest, IdentifyMessageTest)
{

};

TEST_F(SessionTest, IdentifyResponseMessageTest)
{

};

TEST_F(SessionTest, ReadMessageTest)
{

};

TEST_F(SessionTest, WriteMessageTest)
{
	
}
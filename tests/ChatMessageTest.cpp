#include <gtest/gtest.h>
#include "../ChatMessage.h"

class ChatMessageTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m = ChatMessage(
			ChatMessage::MessageType::broadcast, 
			"sender1", 
			{"receiver1", "receiver2"}, 
			"Some text."
		);
	}

	ChatMessage m;
	ChatMessage m2;
};

TEST_F(ChatMessageTest, AreLengthFunctionsOk)
{
	EXPECT_EQ(m.getRecvNum(), 2);
	EXPECT_EQ(m.getLength(), 42);
}

TEST_F(ChatMessageTest, IsEncodeMessageFunctionOk)
{
	std::vector<char> e = m.encodeMessage();
	std::string e_str(e.begin(), e.end());
	EXPECT_EQ(e_str, std::string("0042b002sender1\x01receiver1\x01receiver2\x01Some text."));
}

TEST_F(ChatMessageTest, IsDecodeHeaderFunctionOk)
{
	std::string h = "0042";
	std::vector h_vec(h.begin(), h.end());
	m2.decodeHeader(h_vec);
	
	EXPECT_EQ(m2.getLength(), 42);
}

TEST_F(ChatMessageTest, IsDecodeBodyFunctionOk)
{
	std::vector receivers {"receiver1", "receiver2"};
	std::string b = "b002sender1\x01receiver1\x01receiver2\x01Some text.";
	std::vector b_vec(b.begin(), b.end());
	ASSERT_EQ(m2.decodeBody(b_vec), true);
	
	EXPECT_EQ(m2.getMessageType(), ChatMessage::MessageType::broadcast);
	EXPECT_EQ(m2.getRecvNum(), 2);
	EXPECT_EQ(m2.getSender(), "sender1");
	for(int i = 0; i < 2; i++)
	{
		EXPECT_EQ(m2.getReceivers().at(i), receivers.at(i));
	}
	EXPECT_EQ(m2.getMsgBody(), "Some text.");
}
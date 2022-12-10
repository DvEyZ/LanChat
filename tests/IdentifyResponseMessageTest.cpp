#include <gtest/gtest.h>
#include "../IdentifyResponseMessage.h"

class IdentifyResponseMessageTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m = IdentifyResponseMessage(IdentifyResponseMessage::Status::ok);
        m2 = IdentifyResponseMessage(IdentifyResponseMessage::Status::auth_failed_bad_credentials);
	}

	IdentifyResponseMessage m;
	IdentifyResponseMessage m2;
};

TEST_F(IdentifyResponseMessageTest, IsEncodeMessageFunctionOk)
{
	std::vector<char> e = m.encodeMessage();
	std::string e_str(e.begin(), e.end());
	EXPECT_EQ(e_str, std::string("00010"));

    e = m2.encodeMessage();
	e_str = std::string(e.begin(), e.end());
	EXPECT_EQ(e_str, std::string("00012"));
}

TEST_F(IdentifyResponseMessageTest, IsDecodeBodyFunctionOk)
{
	std::string b = "2";
	std::vector b_vec(b.begin(), b.end());
	ASSERT_EQ(m2.decodeBody(b_vec), true);
	
    EXPECT_EQ(m2.getStatus(), IdentifyResponseMessage::Status::auth_failed_bad_credentials);
}
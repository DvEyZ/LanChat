#include <gtest/gtest.h>
#include "../IdentifyMessage.h"

class IdentifyMessageTest : public ::testing::Test
{
protected:
	void SetUp() override
	{
		m = IdentifyMessage("user", "password");
	}

	IdentifyMessage m;
	IdentifyMessage m2;
};

TEST_F(IdentifyMessageTest, IsEncodeMessageFunctionOk)
{
	std::vector<char> e = m.encodeMessage();
	std::string e_str(e.begin(), e.end());
	EXPECT_EQ(e_str, std::string("0013user\x01password"));
}

TEST_F(IdentifyMessageTest, IsDecodeBodyFunctionOk)
{
	std::string b = "user\x01password";
	std::vector b_vec(b.begin(), b.end());
	ASSERT_EQ(m2.decodeBody(b_vec), true);
	
	EXPECT_EQ(m2.getUsername(), "user");
	EXPECT_EQ(m2.getPassword(), "password");
}
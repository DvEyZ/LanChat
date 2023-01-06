#include <gtest/gtest.h>
#include "../message/IdentifyMessage.h"

class IdentifyMessageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m = new IdentifyMessage("user", "pass");
    }

    void TearDown() override
    {
        delete m;
    }
    IdentifyMessage* m;
};

TEST_F(IdentifyMessageTest, AreSpecificGettersWorking)
{
    auto u = m->getUsername();
    auto p = m->getPassword();

    ASSERT_EQ(u, "user");
    ASSERT_EQ(p, "pass");
}

TEST_F(IdentifyMessageTest, AreBaseGettersWorking)
{
    auto v = m->getVersion();
    auto t = m->getTimestamp();
    auto o = m->getType();

    ASSERT_EQ(v, "1.0");
    ASSERT_EQ(o, "identify");
    std::cerr << t.count() << "\n";
}

TEST_F(IdentifyMessageTest, IsEncodeWorking)
{
    auto x = m->encode();
    std::cerr << x << "\n";
    IdentifyMessage m2;
    m2.decode(x);
    std::cerr << m2.encode() << "\n";
}
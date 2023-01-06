#include <gtest/gtest.h>
#include "../message/SendMessage.h"

class SendMessageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m = new SendMessage("sender", {"receiver1", "receiver2"}, ReadableMessageBody("hello"), SendMessage::TYPE_TARGET);
    }

    void TearDown() override
    {
        delete m;
    }
    SendMessage* m;
};

TEST_F(SendMessageTest, AreSpecificGettersWorking)
{
    auto s = m->getSender();
    auto r = m->getReceivers();
    auto b = m->getBody();
    auto t = m->getSendMessageType();

    ASSERT_EQ(s, "sender");
    ASSERT_EQ(r[0], "receiver1");
    ASSERT_EQ(r[1], "receiver2");
    ASSERT_EQ(b.getType(), "text");
    ASSERT_EQ(b.getContent(), "hello");
    ASSERT_EQ(t, SendMessage::TYPE_TARGET);
}

TEST_F(SendMessageTest, AreBaseGettersWorking)
{
    auto v = m->getVersion();
    auto t = m->getTimestamp();
    auto o = m->getType();

    ASSERT_EQ(v, "1.0");
    ASSERT_EQ(o, "send");
    std::cerr << t.count() << "\n";
}

TEST_F(SendMessageTest, IsEncodeWorking)
{
    auto x = m->encode();
    std::cerr << x << "\n";
    std::this_thread::sleep_for(std::chrono::seconds(5));
    SendMessage m2;
    m2.decode(x);
    std::cerr << m2.encode() << "\n";
}
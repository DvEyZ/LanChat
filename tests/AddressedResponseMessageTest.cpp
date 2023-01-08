#include <gtest/gtest.h>
#include "../message/AddressedResponseMessage.h"

class AddressedResponseMessageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m = new AddressedResponseMessage({"r1", "r2"}, ReadableMessageBody("Your message was not delivered."), ResponseMessage::DENIED);
    }

    void TearDown() override
    {
        delete m;
    }
    Message* m;
};

TEST_F(AddressedResponseMessageTest, AreSpecificGettersWorking)
{
    auto as_resp = dynamic_cast <ResponseMessage*> (m);
    auto as_addr = dynamic_cast <Addressed*> (m);
    ASSERT_EQ(as_resp->ok(), false);
    ASSERT_EQ(as_resp->getBody().getContent(), "Your message was not delivered.");
    ASSERT_EQ(as_addr->getReceivers()[0], "r1");
}

TEST_F(AddressedResponseMessageTest, AreBaseGettersWorking)
{
    auto v = m->getVersion();
    auto t = m->getTimestamp();
    auto o = m->getType();

    ASSERT_EQ(v, "1.0");
    ASSERT_EQ(o, "response");
    std::cerr << t.count() << "\n";
}

TEST_F(AddressedResponseMessageTest, IsEncodeWorking)
{
    auto x = m->encode();
    std::cerr << x << "\n";
    AddressedResponseMessage m2;
    m2.decode(x);
    std::cerr << m2.encode() << "\n";
}
#include <gtest/gtest.h>
#include "../message/ResponseMessage.h"

class ResponseMessageTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m = new ResponseMessage({"r1"}, ReadableMessageBody("Your message was not delivered."), ResponseMessage::DENIED);
    }

    void TearDown() override
    {
        delete m;
    }
    Message* m;
};

TEST_F(ResponseMessageTest, AreSpecificGettersWorking)
{
    auto sm = dynamic_cast <ResponseMessage*> (m);
    
    ASSERT_EQ(sm->ok(), false);
}

TEST_F(ResponseMessageTest, AreBaseGettersWorking)
{
    auto v = m->getVersion();
    auto t = m->getTimestamp();
    auto o = m->getType();

    ASSERT_EQ(v, "1.0");
    ASSERT_EQ(o, "response");
    std::cerr << t.count() << "\n";
}

TEST_F(ResponseMessageTest, IsEncodeWorking)
{
    auto x = m->encode();
    std::cerr << x << "\n";
    ResponseMessage m2;
    m2.decode(x);
    std::cerr << m2.encode() << "\n";
}
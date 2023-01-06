#include <gtest/gtest.h>
#include "../MessageWrapper.h"
#include "../message/SendMessage.h"

class MessageWrapperTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m = new SendMessage("s", {"r1", "r2"}, ReadableMessageBody("Hello World!"), SendMessage::TYPE_TARGET);
        w = MessageWrapper(*m);
    }

    void TearDown() override
    {
        delete m;
    }

    Message* m;
    MessageWrapper w;
};

TEST_F(MessageWrapperTest, IsConstructorOk)
{
    ASSERT_EQ(w.getData(), m->encode());
    ASSERT_EQ(w.getLength(), m->encode().size());
}

TEST_F(MessageWrapperTest, IsEncodeAndDecodeOk)
{
    auto encoded = w.encode();
    MessageWrapper w2;
    w2.decode(encoded);
    ASSERT_EQ(w.getLength(), w2.getLength());
    ASSERT_EQ(w.getData(), w2.getData());

    Message* m2 = new SendMessage();
    m2->decode(w2.getData());
    std::cerr << m->encode() << "\n" << m2->encode() << "\n";
}
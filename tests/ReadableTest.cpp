#include <gtest/gtest.h>
#include "../message/ResponseMessage.h"
#include "../message/SendMessage.h"

class ReadableTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        s = new SendMessage("sender", {"receiver"}, { "Hello, world!" }, SendMessage::TYPE_TARGET);
        r = new ResponseMessage({ "Hello, world!" });
    }

    void TearDown() override
    {
        delete s;
        delete r;
    }
    Readable* s;
    Readable* r;
};

TEST_F(ReadableTest, IsPolymorphismWorking)
{
    auto s_body = s->getBody().getContent();
    auto r_body = r->getBody().getContent();

    ASSERT_EQ(s_body, r_body);

    std::cerr << s->encode() << "\n" << r->encode() << "\n";
}
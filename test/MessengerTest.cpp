#include <limits.h>
#include <gtest/gtest.h>
#include <Messenger.h>

class MessengerTest: public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }
};

// Могу отправить сообщение любому человеку из списка контактов
TEST_F(MessengerTest, ICanToSendMessegeToAnyoneInMyContactList) {

    //EXPECT_EQ(11, Addition::twoValues(5,6));
}

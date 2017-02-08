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
   // UserSharedPtr me = std::make_shared<User>("my@email.com", Date())

    //EXPECT_EQ(11, Addition::twoValues(5,6));
}

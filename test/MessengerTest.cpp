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

    std::string alex_email { "alex@email.com"};
    std::string tim_email { "tim@email.com"};

    UserSharedPtr me = std::make_shared<User>("my@email.com", Date("1", "may", "1980"));
    UserSharedPtr alex = std::make_shared<User>(alex_email, Date("10", "july", "1990"));
    UserSharedPtr tim = std::make_shared<User>(tim_email, Date("27", "march", "1987"));

    me->addContact(alex->getMyContact());
    me->addContact(tim->getMyContact());

    me->sendMessege(tim_email, "Hello Tim!!!");

    //EXPECT_EQ(11, Addition::twoValues(5,6));
}

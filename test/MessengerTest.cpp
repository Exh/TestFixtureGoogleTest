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
    std::string my_email { "my@email.com" };

    UserSharedPtr me = std::make_shared<User>(my_email, Date("1", "may", "1980"));
    UserSharedPtr alex = std::make_shared<User>(alex_email, Date("10", "july", "1990"));
    UserSharedPtr tim = std::make_shared<User>(tim_email, Date("27", "march", "1987"));

    me->addContact(alex->getMyContact());
    me->addContact(tim->getMyContact());

    me->sendMessege(tim_email, "Hello Tim!!!");

    std::string expected { "my@email.com wrote: Hello Tim!!!" };
    std::string result { tim->readP2PChat(my_email) };
    EXPECT_EQ(expected, result);
}

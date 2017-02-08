#include <limits.h>
#include <gtest/gtest.h>
#include <Messenger.h>

class MessengerTest: public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }

    static UserSharedPtr createMe() {
        return createUser(createMyEmail(), Date("1", "may", "1980"));
    }

    static std::string createMyEmail() { return "my@email.com"; }

    static UserSharedPtr createUser(const std::string& name, const Date& date) {
        return std::make_shared<User>(name, date);
    }
};

// Могу отправить сообщение любому человеку из списка контактов
TEST_F(MessengerTest, ICanToSendMessegeToAnyoneInMyContactList) {

    std::string alex_email { "alex@email.com"};
    std::string tim_email { "tim@email.com"};
    std::string my_email = MessengerTest::createMyEmail();

    UserSharedPtr me = MessengerTest::createMe();
    UserSharedPtr alex = MessengerTest::createUser(alex_email, Date("10", "july", "1990"));
    UserSharedPtr tim = std::make_shared<User>(tim_email, Date("27", "march", "1987"));

    me->addContact(alex->getMyContact());
    me->addContact(tim->getMyContact());

    me->sendMessege(tim_email, "Hello Tim!!!");

    std::string expected =  + "You wrote: Hello Tim!!!";
    std::string result = me->readP2PChat(tim_email);
    EXPECT_EQ(expected, result);
}

//Могу получать сообщения от других пользоватлей
TEST_F(MessengerTest, ICanToRecieveMessegeFromAnyOne) {

    std::string alex_email { "alex@email.com"};
    std::string tim_email { "tim@email.com"};
    std::string my_email { "my@email.com" };

    UserSharedPtr me = std::make_shared<User>(my_email, Date("1", "may", "1980"));
    UserSharedPtr alex = std::make_shared<User>(alex_email, Date("10", "july", "1990"));
    UserSharedPtr tim = std::make_shared<User>(tim_email, Date("27", "march", "1987"));

    tim->addContact(me->getMyContact());

    tim->sendMessege(my_email, "Was Up?");

    std::string expected = tim_email + " wrote: Was Up?";
    std::string result = me->readP2PChat(tim_email);
    EXPECT_EQ(expected, result);
}

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
        return createUser(createMyEmail(), Date(1, 6, 1980));
    }

    static UserSharedPtr createAlex() {
        return createUser(createAlexEmail(), Date(10, 7, 1990));
    }

    static UserSharedPtr createTim() {
        return createUser(createTimEmail(), Date(27, 3, 1987));
    }

    static std::string createAlexEmail() { return "alex@email.com"; }
    static std::string createTimEmail() { return "tim@email.com"; }
    static std::string createMyEmail() { return "my@email.com"; }

    static UserSharedPtr createUser(const std::string& name, const Date& date) {
        return std::make_shared<User>(name, date);
    }
};

// Могу отправить сообщение любому человеку из списка контактов
TEST_F(MessengerTest, ICanToSendMessegeToAnyoneInMyContactList) {
    std::string tim_email  = MessengerTest::createTimEmail();
    UserSharedPtr me = MessengerTest::createMe();
    UserSharedPtr tim = MessengerTest::createTim();

    me->addContact(tim->getMyContact());
    me->sendMessege(tim_email, "Hello Tim!!!");

    std::string expected =  + "You wrote: Hello Tim!!!";
    std::string result = me->readP2PChat(tim_email);
    EXPECT_EQ(expected, result);
}

//Могу получать сообщения от других пользоватлей
TEST_F(MessengerTest, ICanToRecieveMessegeFromAnyOne) {
    std::string tim_email  = MessengerTest::createTimEmail();
    std::string my_email   = MessengerTest::createMyEmail();
    UserSharedPtr me = MessengerTest::createMe();
    UserSharedPtr tim = MessengerTest::createTim();

    tim->addContact(me->getMyContact());

    tim->sendMessege(my_email, "Was Up?");

    std::string expected = tim_email + " wrote: Was Up?";
    std::string result = me->readP2PChat(tim_email);
    EXPECT_EQ(expected, result);
}

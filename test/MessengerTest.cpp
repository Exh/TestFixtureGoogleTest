#include <limits.h>
#include <gtest/gtest.h>
#include <Messenger.h>

class MessengerTest: public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {

    }

    static UserSharedPtr getMe() {
        static auto me = createUser(getMyEmail(), Date(1, 6, 1980));
        return me;
    }

    static UserSharedPtr getAlex() {
        static auto alex = createUser(getAlexEmail(), Date(10, 7, 1990));
        return alex;
    }

    static UserSharedPtr getTim() {
        static auto tim = createUser(getTimEmail(), Date(27, 3, 1987));
        return tim;
    }

    static std::string getAlexEmail() { return "alex@email.com"; }
    static std::string getTimEmail() { return "tim@email.com"; }
    static std::string getMyEmail() { return "my@email.com"; }

    static UserSharedPtr createUser(const std::string& name, const Date& date) {
        return std::make_shared<User>(name, date);
    }
};

// Могу отправить сообщение любому человеку из списка контактов
TEST_F(MessengerTest, ICanToSendMessegeToAnyoneInMyContactList) {
    std::string tim_email  = MessengerTest::getTimEmail();
    UserSharedPtr me = MessengerTest::getMe();
    UserSharedPtr tim = MessengerTest::getTim();

    me->addContact(tim->getMyContact());
    me->sendMessege(tim_email, "Hello Tim!!!");

    std::string expected =  + "You wrote: Hello Tim!!!";
    std::string result = me->readP2PChat(tim_email);
    EXPECT_EQ(expected, result);
}

//Могу получать сообщения от других пользоватлей
TEST_F(MessengerTest, ICanToRecieveMessegeFromAnyOne) {
    std::string tim_email  = MessengerTest::getTimEmail();
    std::string my_email   = MessengerTest::getMyEmail();
    UserSharedPtr me = MessengerTest::getMe();
    UserSharedPtr tim = MessengerTest::getTim();

    tim->addContact(me->getMyContact());

    tim->sendMessege(my_email, "Was Up?");

    std::string expected = tim_email + " wrote: Was Up?";
    std::string result = me->readP2PChat(tim_email);
    EXPECT_EQ(expected, result);
}

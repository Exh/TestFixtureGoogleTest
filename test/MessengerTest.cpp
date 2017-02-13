#include <limits.h>
#include <gtest/gtest.h>
#include <Messenger.h>


class TestForDateUser: public User
{
public:
    TestForDateUser(std::string email, const Date& birth_day):User(email, birth_day) {}
    void SetDate(Date d) { m_test_date = d; }
    Date getCurrentDate() const { return m_test_date; }
private:
    Date m_test_date = {1, 2, 2017};
};


class MessengerTest: public ::testing::Test {
protected:
    virtual void SetUp() {

    }

    virtual void TearDown() {
        getMe()->clearContacts();
        getAlex()->clearContacts();
        getTim()->clearContacts();
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

    static UserSharedPtr createUserForTestDate(const std::string& name, const Date& birth_date, const Date& current_date) {
        auto user = std::make_shared<TestForDateUser>(name, birth_date);
        user->SetDate(current_date);
        return user;

    }
    static std::string getAlexEmail() { return "alex@email.com"; }
    static std::string getTimEmail() { return "tim@email.com"; }
    static std::string getMyEmail() { return "my@email.com"; }

    static UserSharedPtr createUser(const std::string& name, const Date& birth_date) {
      return std::make_shared<User>(name, birth_date);
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

TEST_F(MessengerTest, IGetMessegeFromTimTodayIsHimBirthDayOpenChatISeeOfferToSendHimGreetingCard) {
    std::string tim_email  = MessengerTest::getTimEmail();
    std::string my_email   = MessengerTest::getMyEmail();
    Date today(18, 2, 2017);
    Date tim_birth(18, 2, 1987);
    UserSharedPtr me = MessengerTest::createUserForTestDate(my_email, Date(2, 2, 1970), today);
    UserSharedPtr tim = MessengerTest::createUser(tim_email, tim_birth);

    me->addContact(tim->getMyContact());
    tim->sendMessege(my_email, "wassup dude?\n");

    me->checkContactsBirthDayToDay();

    std::string expected = tim_email + " wrote: wassup dude?\nToday is " + tim_email + " birthday!!!\nDo you wanna send a greeting card?\n";
    std::string result = me->readP2PChat(tim_email);    
    EXPECT_EQ(expected, result);
}

TEST_F(MessengerTest, ICanCreateAChatRoomThenAddingContactThenReadChat) {
    UserSharedPtr me = MessengerTest::getMe();
    UserSharedPtr tim = MessengerTest::getTim();
    std::string tim_email  = MessengerTest::getTimEmail();
    me->addContact(tim->getMyContact());

    size_t hash_room = me->createChatRoom();
    me->addContactToChat(hash_room, tim);

    std::string expected ="You have invited " + tim_email + "\n";
    std::string result = me->readChatRoom(hash_room);
    EXPECT_EQ(expected, result);
}

TEST_F(MessengerTest, ICanCreateAChatRoomThenAddingContactsSendThemMessage) {
    UserSharedPtr me = MessengerTest::getMe();
    UserSharedPtr tim = MessengerTest::getTim();
    UserSharedPtr alex = MessengerTest::getAlex();
    me->addContact(tim->getMyContact());
    me->addContact(alex->getMyContact());

    size_t hash_room = me->createChatRoom();
    me->addContactToChat(hash_room, tim);
    me->addContactToChat(hash_room, alex);
    me->sendChatRoomMessege(hash_room, "Hi there");


    std::string expected_tim = "my@email.com have invited You\nmy@email.com has invited alex@email.com\nmy@email.com wrote: Hi there";
    std::string result_tim   = tim->readChatRoom(hash_room);
    EXPECT_EQ(expected_tim, result_tim);

    std::string expected_alex = "my@email.com have invited You\nmy@email.com wrote: Hi there";
    std::string result_alex   = alex->readChatRoom(hash_room);
    EXPECT_EQ(expected_alex, result_alex);
}

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

// Если у моего собеседника день рождения, месенжер предлагает отправить ему картинку с поздравлением или стикер
TEST_F(MessengerTest, ICanToRecieveListAllMyContactsWhoHasBirthDayToday) {
    std::string tim_email  = MessengerTest::getTimEmail();
    std::string my_email   = MessengerTest::getMyEmail();
    Date today(18, 2, 2017);
    Date tim_birth(18, 2, 1987);
    UserSharedPtr me = MessengerTest::createUserForTestDate(my_email, Date(2, 2, 1970), today);
    UserSharedPtr tim = MessengerTest::createUser(tim_email, tim_birth);
    me->addContact(tim->getMyContact());

//    Date dm = me->getCurrentDate();
//    std::cout << "My date: " << dm.getDay() << " " << dm.getMonth() << " " << dm.getYear() << std::endl;
//    Date dt = tim->getCurrentDate();
//    std::cout << "Tim date: " << dt.getDay() << " " << dt.getMonth() << " " << dt.getYear() << std::endl;

    bool expected = true;
    bool result = !me->checkContactsBirthDayToDay().empty();
    EXPECT_EQ(expected, result);
}

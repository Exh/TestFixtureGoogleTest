#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <stdint.h>
#include <unordered_map>
#include <memory>
#include <string>

/*
Мессенжер (на примере WhatsApp, Viber, Telegram)
* У меня есть список контактов, с кем я могу общаться
* Могу отправить сообщение любому человеку из списка контактов
* Могу создать чат с несколькими людьми и отправить сообщения им
* Могу получать сообщения от других пользоватлей
* Если у моего собеседника день рождения, месенжер предлагает отправить ему картинку с поздравлением или стикер
*/


class Date
{
    Date(char day, char month, char year):
        day(day), month(month), year(year)
    { }

    char getDay() const { return day; }
    char getMonth() const { return month; }
    char getYear() const { return year; }
private:
    char day;
    char month;
    char year;
};

class Contact
{
public:
    Contact(std::string email, Date birth_date);
private:
    std::string m_email;
    Date        m_birth_date;
};

typedef std::shared_ptr<Contact> ContactSharedPtr;

class ChatRoom
{

};

class User
{
public:
    User(std::string email, const Date& birth_date);
private:
    std::unordered_map<std::string, ContactSharedPtr> m_contacts;   // email,
    ContactSharedPtr                                  m_my_contact; //
};

typedef std::shared_ptr<User> UserCharedPtr;



#endif

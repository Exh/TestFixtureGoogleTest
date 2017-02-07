#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <stdint.h>
#include <unordered_map>

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
private:
    std::uint64_t m_id;
    Date m_date;
};

class ChatRoom
{

};

class User
{
private:
    std::unordered_map<Contact> m_contacts;
    Contact my;


};




#endif

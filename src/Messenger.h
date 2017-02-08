#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <stdint.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

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
public:
    Date(std::string day, std::string month, std::string year):
        day(day), month(month), year(year)
    { }

    std::string getDay() const { return day; }
    std::string getMonth() const { return month; }
    std::string getYear() const { return year; }
private:
    std::string day;
    std::string month;
    std::string year;
};


class Contact;
typedef std::weak_ptr<Contact> ContactWeakPtr;

class Contact: public std::enable_shared_from_this<Contact>
{
public:
    Contact(std::string email, Date birth_day);
    std::string getEmail() const { return m_email; }
    Date        getBirthDay() const { return m_birth_day; }

    virtual void createP2PChat(uint64_t id, ContactWeakPtr creator) = 0;
    virtual void recieveMessege(uint64_t id, const std::string& text) = 0;
private:
    std::string m_email;
    Date        m_birth_day;

};

typedef std::shared_ptr<Contact> ContactSharedPtr;



class IChat
{
public:
    IChat(uint64_t id):m_id(id) { }
    std::string getText() const { return m_text; }
protected:
    std::string m_text;
    uint64_t m_id;
};

class P2PChat: public IChat
{
public:
    P2PChat(ContactWeakPtr collocutor, uint64_t id, ContactWeakPtr creator):
        IChat(id),
        m_collocutor(collocutor)
    {
        auto col =  m_collocutor.lock();
        if (col)
        {
            col->createP2PChat(m_id, creator);
        }
    }
    void sendMessege(const std::string& text);
    void recieveMessege(const std::string& text);
private:
    ContactWeakPtr m_collocutor;
};

typedef std::shared_ptr<P2PChat> P2PChatSharedPtr;

//class ConferenceChat
//{
//    std::vector<ContactWeakPtr> m_contacts;
//};

class User: public Contact
{
public:
    User(std::string email, const Date& birth_day);
    ContactWeakPtr getMyContact() { ContactSharedPtr me = shared_from_this(); return me; }
    void addContact(ContactWeakPtr newContact);
    void sendMessege(const std::string& reciever, const std::string& text);
    std::string getCurrentTime() const;

    void createP2PChat(uint64_t id, ContactWeakPtr creator);
    void recieveMessege(uint64_t id, const std::string& text);

    std::string readChat(uint64_t id) const;
    std::string readP2PChat(std::string& contact_email);
private:
    size_t generateHash() const;
    std::unordered_map<std::string, ContactWeakPtr>         m_contacts;   // email,
    std::unordered_map<std::string, P2PChatSharedPtr>       m_p2p_chats;
};

typedef std::shared_ptr<User> UserSharedPtr;



#endif

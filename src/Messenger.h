
#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <stdint.h>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <iostream>

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
    Date(int day, int month, int year);
    int getDay()   const { return   m_day; }
    int getMonth() const { return m_month; }
    int getYear()  const { return  m_year; }
private:
    int m_day;
    int m_month;
    int m_year;
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
    virtual void createChatRoom(uint64_t id, const std::string& text, const std::vector<ContactWeakPtr>& creator) = 0;
    virtual void recieveMessege(uint64_t id, const std::string& text) = 0;
    virtual void addContactToChatRoom(uint64_t id, ContactWeakPtr newContact, bool initiate = false) = 0;
    virtual ~Contact() { }

private:
    std::string m_email;
    Date        m_birth_day;

};

typedef std::shared_ptr<Contact> ContactSharedPtr;



class IChat
{
public:
    IChat(uint64_t id, const std::string& text = ""):m_id(id), m_text(text) { }
    std::string getText() const { return m_text; }
protected:
    std::string m_text;
    uint64_t m_id;
};

class P2PChat: public IChat
{
public:
    P2PChat(ContactWeakPtr collocutor, uint64_t id, ContactWeakPtr creator, bool initiated = false);
    void sendMessege(const std::string& text);
    void recieveMessege(const std::string& text);
    void systemMessage(const std::string& text);
private:
    ContactWeakPtr m_collocutor;
};

typedef std::shared_ptr<P2PChat> P2PChatSharedPtr;

class ChatRoom: public IChat
{
public:
    ChatRoom(uint64_t id, ContactWeakPtr owner,
             const std::string& text = "",
             const std::vector<ContactWeakPtr>& collocutors = std::vector<ContactWeakPtr>());
    void sendMessege(const std::string& text);
    void recieveMessege(const std::string& text);
    void systemMessage(const std::string& text);
    void addContact(ContactWeakPtr collocutor, bool init=false);
    void removeContact(ContactWeakPtr me);
private:
    ContactWeakPtr m_owner;
    std::vector<ContactWeakPtr> m_collocutors;

};

typedef std::shared_ptr<ChatRoom> ChatRoomSharedPtr;

class User: public Contact
{
public:
    User(std::string email, const Date& birth_day);
    ContactWeakPtr getMyContact() { ContactSharedPtr me = shared_from_this(); return me; }
    void addContact(ContactWeakPtr newContact);
    size_t createChatRoom();
    void addContactToChat(uint64_t id, ContactWeakPtr newContact);
    void sendMessege(const std::string& reciever, const std::string& text);
    void sendChatRoomMessege(uint64_t id, const std::string& text);

    std::string getCurrentTimeStr() const;
    virtual Date getCurrentDate() const;

    void createP2PChat(uint64_t id, ContactWeakPtr creator);
    void createChatRoom(uint64_t id, const std::string& text = "", const std::vector<ContactWeakPtr>& creator = std::vector<ContactWeakPtr>());
    void recieveMessege(uint64_t id, const std::string& text);

    void addContactToChatRoom(uint64_t id, ContactWeakPtr newContact, bool init = false);

    std::string readP2PChat(std::string& contact_email) const;
    std::string readChatRoom(size_t) const;
    void clearContacts();
    std::vector<std::string> checkContactsBirthDayToDay() const;
private:
    size_t generateHash(const std::string& s) const;
    void createChatP2P(ContactWeakPtr newContact, size_t hash, bool initiated = false);
    void hiddenAddContact(ContactWeakPtr newContact);
    std::unordered_map<std::string, ContactWeakPtr>         m_contacts;   // email,
    std::unordered_map<std::string, P2PChatSharedPtr>       m_p2p_chats;
    std::unordered_map<size_t, P2PChatSharedPtr>            m_p2p_chats_ids;
    std::unordered_map<size_t, ChatRoomSharedPtr>           m_chat_rooms;
};

typedef std::shared_ptr<User> UserSharedPtr;



#endif

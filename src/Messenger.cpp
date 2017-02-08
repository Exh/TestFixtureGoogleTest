#include "Messenger.h"
#include <iomanip>
#include <ctime>
#include <functional>
#include <sstream>
#include <algorithm>

Contact::Contact(std::string email, Date birth_day):
    m_email(email), m_birth_day(birth_day)
{

}

User::User(std::string email, const Date& birth_day):Contact(email, birth_day)
{

}

void User::addContact(ContactWeakPtr newContact)
{
    hiddenAddContact(newContact);

    auto newCon  = newContact.lock();
    size_t hash = generateHash(getEmail() + newCon->getEmail() + getCurrentTimeStr());

    createChatRoom(newContact, hash, true);
}

void User::hiddenAddContact(ContactWeakPtr newContact)
{
    auto newCon  = newContact.lock();
    m_contacts[newCon->getEmail()] = newContact;
}

void User::sendMessege(const std::string &reciever, const std::string &text)
{
    auto chat = m_p2p_chats[reciever];
    chat->sendMessege(text);
}

void User::createChatRoom(ContactWeakPtr newContact, size_t hash, bool initiated)
{
    auto newCon = newContact.lock();
    if (newCon) {
        P2PChatSharedPtr chat = std::make_shared<P2PChat>(newContact, hash, getMyContact(), initiated);
        m_p2p_chats[newCon->getEmail()] = chat;
        m_p2p_chats_ids[hash] = chat;
    }
}

size_t User::generateHash(const std::string& s) const
{
    std::hash<std::string> hash_fn;
    return hash_fn(s);
}


std::string User::getCurrentTimeStr() const
{
    time_t seconds;
    time(&seconds);
    std::stringstream ss;
    ss << seconds;
    return ss.str();
}

Date User::getCurrentDate() const
{
    time_t seconds;
    time(&seconds);
    struct tm * now = localtime(&seconds);

    return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

void User::createP2PChat(uint64_t id, ContactWeakPtr creator)
{
    hiddenAddContact(creator);
    createChatRoom(creator, id);
}

void User::recieveMessege(uint64_t id, const std::string &text)
{
    auto iter = m_p2p_chats_ids.find(id);
    if (iter != m_p2p_chats_ids.end()) {
        return iter->second->recieveMessege(text);
    }
}



std::string User::readP2PChat(std::string &contact_email)
{
    auto iter = m_p2p_chats.find(contact_email);
    if (iter != m_p2p_chats.end()) {
        return iter->second->getText();
    }
    return "";
}

void User::clearContacts()
{
    m_contacts.clear();
    m_p2p_chats.clear();
    m_p2p_chats_ids.clear();
}

std::vector<std::string> User::checkContactsBirthDayToDay() const
{
    std::vector<std::string> birth_day_list;
    Date today = getCurrentDate();
    std::for_each(std::begin(m_contacts), std::end(m_contacts),
        [&] (const std::unordered_map<std::string, ContactWeakPtr>::value_type& p)
        {
             auto contact = p.second.lock();
             auto birth_day = contact->getBirthDay();

             if ((birth_day.getDay() == today.getDay()) &&
                 (birth_day.getMonth() == today.getMonth()))
             {
                birth_day_list.push_back(p.first);
             }
        });

    return birth_day_list;
}

P2PChat::P2PChat(ContactWeakPtr collocutor, uint64_t id, ContactWeakPtr creator, bool initiated):
    IChat(id),
    m_collocutor(collocutor)
{
    if (initiated) {
        auto col =  m_collocutor.lock();
        if (col)
            col->createP2PChat(m_id, creator);
    }
}

void P2PChat::sendMessege(const std::string &text)
{
    auto collocutor = m_collocutor.lock();
    if (collocutor) {
        collocutor->recieveMessege(m_id, text);
        m_text += "You wrote: " + text;
    }
}

void P2PChat::recieveMessege(const std::string &text)
{
    auto collocutor = m_collocutor.lock();
    if (collocutor) {
        m_text += collocutor->getEmail() + " wrote: " + text;
    }
}


Date::Date(int day, int month, int year):
    m_day(day), m_month(month), m_year(year)
{

}

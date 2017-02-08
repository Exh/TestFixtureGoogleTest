#include "Messenger.h"
#include <iomanip>
#include <ctime>
#include <functional>
#include <sstream>

Contact::Contact(std::string email, Date birth_day):
    m_email(email), m_birth_day(birth_day)
{

}

User::User(std::string email, const Date& birth_day):Contact(email, birth_day)
{

}

void User::addContact(ContactWeakPtr newContact)
{
    auto newCon  = newContact.lock();
    m_contacts[newCon->getEmail()] = newContact;

    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(getEmail() + getCurrentTime());
    m_p2p_chats[newCon->getEmail()] = std::make_shared<P2PChat>(newContact, str_hash, getMyContact());
}

void User::sendMessege(const std::string &reciever, const std::string &text)
{
    auto chat = m_p2p_chats[reciever];
    chat->sendMessege(text);
}




std::string User::getCurrentTime()
{
    auto t = std::time(nullptr);
    time_t seconds;
    time(&seconds);
    std::stringstream ss;
    ss << seconds;
    return ss.str();
}

void User::createP2PChat(uint64_t id, ContactWeakPtr creator)
{
}

void User::recieveMessege(uint64_t id, const std::string &text)
{
}

std::string User::readChat(uint64_t id) const
{
//    auto iter = m_p2p_chats.find(id);
//    if (iter != m_p2p_chats.end()) {
        //iter->second->
//    }
}

void P2PChat::sendMessege(const std::string &text)
{
    auto collocutor = m_collocutor.lock();
    if (collocutor)
        collocutor->recieveMessege(m_id, text);
}

void P2PChat::recieveMessege(const std::string &text)
{
    m_text.append(text.begin(), text.end());
}

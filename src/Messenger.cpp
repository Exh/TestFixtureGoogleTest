#include "Messenger.h"
#include <iomanip>
#include <ctime>
#include <functional>
#include <sstream>

Contact::Contact(std::string email, Date birth_day):
    m_email(email), m_birth_day(birth_day)
{

}

User::User(std::string email, const Date& birth_day)
{
    m_my_contact = std::make_shared<Contact>(email, birth_day);
}

void User::addContact(ContactWeakPtr newContact)
{
    auto newCon  = newContact.lock();
    m_contacts[newCon->getEmail()] = newContact;

    std::hash<std::string> hash_fn;
    size_t str_hash = hash_fn(m_my_contact->getEmail() + getCurrentTime());
    m_p2p_chats[newCon->getEmail()] = std::make_shared<P2PChat>(newContact, str_hash);
}

void User::sendMessege(const std::string &reciever, const std::string &text)
{

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



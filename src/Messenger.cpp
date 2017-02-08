#include "Messenger.h"

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
}

void User::sendMessege(const std::string &reciever, const std::string &text)
{
}

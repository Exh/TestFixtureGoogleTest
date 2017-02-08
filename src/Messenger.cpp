#include "Messenger.h"

Contact::Contact(std::string email, Date birth_date):
    m_email(email), m_birth_date(birth_date)
{

}

User::User(std::string email, const Date& birth_date)
{
    m_my_contact = std::make_shared<Contact>(email, birth_date);
}

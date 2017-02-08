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
    hiddenAddContact(newContact);

    auto newCon  = newContact.lock();
    size_t hash = generateHash(getEmail() + newCon->getEmail() + getCurrentTime());

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


std::string User::getCurrentTime() const
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

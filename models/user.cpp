#include "user.h"

User::User()
    : m_id(-1)
{
}

User::User(int id, const QString &username, const QDateTime &createdAt,
           const QDateTime &lastLogin)
    : m_id(id)
    , m_username(username)
    , m_createdAt(createdAt)
    , m_lastLogin(lastLogin)
{
}

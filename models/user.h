#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class User
{
public:
    User();
    User(int id, const QString &username, const QDateTime &createdAt,
         const QDateTime &lastLogin);

    // Getters
    int id() const { return m_id; }
    QString username() const { return m_username; }
    QDateTime createdAt() const { return m_createdAt; }
    QDateTime lastLogin() const { return m_lastLogin; }

    // Setters
    void setId(int id) { m_id = id; }
    void setUsername(const QString &username) { m_username = username; }
    void setCreatedAt(const QDateTime &createdAt) { m_createdAt = createdAt; }
    void setLastLogin(const QDateTime &lastLogin) { m_lastLogin = lastLogin; }

private:
    int m_id;
    QString m_username;
    QDateTime m_createdAt;
    QDateTime m_lastLogin;
};

#endif // USER_H

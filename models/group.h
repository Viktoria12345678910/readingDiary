#ifndef GROUP_H
#define GROUP_H

#include <QString>
#include <QList>

class Group
{
public:
    Group();
    Group(int id, const QString &groupName);

    // Getters
    int id() const { return m_id; }
    QString groupName() const { return m_groupName; }
    QList<int> userIds() const { return m_userIds; }

    // Setters
    void setId(int id) { m_id = id; }
    void setGroupName(const QString &groupName) { m_groupName = groupName; }
    void setUserIds(const QList<int> &userIds) { m_userIds = userIds; }
    void addUserId(int userId) { m_userIds.append(userId); }

private:
    int m_id;
    QString m_groupName;
    QList<int> m_userIds;
};

#endif // GROUP_H

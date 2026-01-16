#include "group.h"

Group::Group()
    : m_id(-1)
{
}

Group::Group(int id, const QString &groupName)
    : m_id(id)
    , m_groupName(groupName)
{
}

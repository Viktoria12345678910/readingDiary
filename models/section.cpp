#include "section.h"

Section::Section()
    : m_id(-1)
    , m_bookCount(0)
    , m_accessGroups("all")
{
}

Section::Section(int id, const QString &name, const QString &abbreviation,
                 const QString &description, const QString &path,
                 int bookCount, const QString &accessGroups)
    : m_id(id)
    , m_name(name)
    , m_abbreviation(abbreviation)
    , m_description(description)
    , m_path(path)
    , m_bookCount(bookCount)
    , m_accessGroups(accessGroups)
{
}

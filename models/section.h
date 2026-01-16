#ifndef SECTION_H
#define SECTION_H

#include <QString>

class Section
{
public:
    Section();
    Section(int id, const QString &name, const QString &abbreviation,
            const QString &description, const QString &path,
            int bookCount, const QString &accessGroups);

    // Getters
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QString abbreviation() const { return m_abbreviation; }
    QString description() const { return m_description; }
    QString path() const { return m_path; }
    int bookCount() const { return m_bookCount; }
    QString accessGroups() const { return m_accessGroups; }

    // Setters
    void setId(int id) { m_id = id; }
    void setName(const QString &name) { m_name = name; }
    void setAbbreviation(const QString &abbreviation) { m_abbreviation = abbreviation; }
    void setDescription(const QString &description) { m_description = description; }
    void setPath(const QString &path) { m_path = path; }
    void setBookCount(int bookCount) { m_bookCount = bookCount; }
    void setAccessGroups(const QString &accessGroups) { m_accessGroups = accessGroups; }

private:
    int m_id;
    QString m_name;
    QString m_abbreviation;
    QString m_description;
    QString m_path;
    int m_bookCount;
    QString m_accessGroups;
};

#endif // SECTION_H

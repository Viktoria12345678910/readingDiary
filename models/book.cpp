#include "book.h"

Book::Book()
    : m_id(-1)
    , m_rating(0)
    , m_read(false)
    , m_sectionId(-1)
{
}

Book::Book(int id, const QString &medium, const QString &title,
           const QString &author, const QString &genre, int rating,
           const QString &description, bool read, const QString &viewRights,
           const QByteArray &cover, int sectionId)
    : m_id(id)
    , m_medium(medium)
    , m_title(title)
    , m_author(author)
    , m_genre(genre)
    , m_rating(rating)
    , m_description(description)
    , m_read(read)
    , m_viewRights(viewRights)
    , m_cover(cover)
    , m_sectionId(sectionId)
{
}

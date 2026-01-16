#ifndef BOOK_H
#define BOOK_H

#include <QString>
#include <QByteArray>

class Book
{
public:
    Book();
    Book(int id, const QString &medium, const QString &title,
         const QString &author, const QString &genre, int rating,
         const QString &description, bool read, const QString &viewRights,
         const QByteArray &cover, int sectionId);

    // Getters
    int id() const { return m_id; }
    QString medium() const { return m_medium; }
    QString title() const { return m_title; }
    QString author() const { return m_author; }
    QString genre() const { return m_genre; }
    int rating() const { return m_rating; }
    QString description() const { return m_description; }
    bool isRead() const { return m_read; }
    QString viewRights() const { return m_viewRights; }
    QByteArray cover() const { return m_cover; }
    int sectionId() const { return m_sectionId; }

    // Setters
    void setId(int id) { m_id = id; }
    void setMedium(const QString &medium) { m_medium = medium; }
    void setTitle(const QString &title) { m_title = title; }
    void setAuthor(const QString &author) { m_author = author; }
    void setGenre(const QString &genre) { m_genre = genre; }
    void setRating(int rating) { m_rating = rating; }
    void setDescription(const QString &description) { m_description = description; }
    void setRead(bool read) { m_read = read; }
    void setViewRights(const QString &viewRights) { m_viewRights = viewRights; }
    void setCover(const QByteArray &cover) { m_cover = cover; }
    void setSectionId(int sectionId) { m_sectionId = sectionId; }

private:
    int m_id;
    QString m_medium;
    QString m_title;
    QString m_author;
    QString m_genre;
    int m_rating;
    QString m_description;
    bool m_read;
    QString m_viewRights;
    QByteArray m_cover;
    int m_sectionId;
};

#endif // BOOK_H

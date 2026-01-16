#include "database.h"
#include <QRandomGenerator>

Database& Database::instance()
{
    static Database instance;
    return instance;
}

Database::Database()
{
}

Database::~Database()
{
    disconnect();
}

bool Database::connect(const QString &path)
{
    mDb = QSqlDatabase::addDatabase("QSQLITE");
    mDb.setDatabaseName(path);

    if (!mDb.open()) {
        qDebug() << "Error: connection with database failed";
        return false;
    }

    qDebug() << "Database: connection ok";
    return createTables();
}

void Database::disconnect()
{
    if (mDb.isOpen()) {
        mDb.close();
    }
}

bool Database::createTables()
{
    QSqlQuery query;

    // Create Sections table
    QString createSections = R"(
        CREATE TABLE IF NOT EXISTS sections (
            section_id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            abbreviation TEXT,
            description TEXT,
            book_count INTEGER DEFAULT 0,
            access_groups TEXT DEFAULT 'all'
        )
    )";

    if (!query.exec(createSections)) {
        qDebug() << "Error creating sections table:" << query.lastError();
        return false;
    }

    // Create Books table
    QString createBooks = R"(
        CREATE TABLE IF NOT EXISTS books (
            book_id INTEGER PRIMARY KEY AUTOINCREMENT,
            medium TEXT NOT NULL,
            title TEXT NOT NULL,
            author TEXT,
            genre TEXT,
            rating INTEGER,
            description TEXT,
            read BOOLEAN DEFAULT 0,
            view_rights TEXT DEFAULT 'everyone',
            cover BLOB,
            section_id INTEGER,
            FOREIGN KEY (section_id) REFERENCES sections(section_id)
        )
    )";

    if (!query.exec(createBooks)) {
        qDebug() << "Error creating books table:" << query.lastError();
        return false;
    }

    QString createUsers = R"(
        CREATE TABLE IF NOT EXISTS users (
            user_id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            salt TEXT NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            last_login DATETIME
        )
    )";

    if (!query.exec(createUsers)) {
        qDebug() << "Error creating users table:" << query.lastError();
        return false;
    }
    else Database::createUser("admin", "123");


    qDebug() << "All tables created successfully";
    return true;
}

QString Database::generateSalt()
{
    QByteArray salt;
    for (int i = 0; i < 16; i++) {
        salt.append(char(QRandomGenerator::global()->bounded(256)));
    }
    return salt.toBase64();
}

QString Database::hashPassword(const QString &password, const QString &salt)
{
    QString saltedPassword = password + salt;
    QByteArray hash = QCryptographicHash::hash(
        saltedPassword.toUtf8(),
        QCryptographicHash::Sha256
    );
    return hash.toHex();
}

bool Database::createUser(const QString &username, const QString &password)
{
    QString salt = generateSalt();
    QString hash = hashPassword(password, salt);

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password_hash, salt) VALUES (?, ?, ?)");
    query.addBindValue(username);
    query.addBindValue(hash);
    query.addBindValue(salt);

    if (!query.exec()) {
        qDebug() << "Error creating user:" << query.lastError();
        return false;
    }

    return true;
}

bool Database::authenticateUser(const QString &username, const QString &password)
{
    QSqlQuery query;
    query.prepare("SELECT password_hash, salt FROM users WHERE username = ?");
    query.addBindValue(username);

    if (!query.exec() || !query.next()) {
        return false;
    }

    QString storedHash = query.value(0).toString();
    QString salt = query.value(1).toString();
    QString computedHash = hashPassword(password, salt);

    if (computedHash == storedHash) {
        // Update last login
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE users SET last_login = ? WHERE username = ?");
        updateQuery.addBindValue(QDateTime::currentDateTime());
        updateQuery.addBindValue(username);
        updateQuery.exec();

        return true;
    }

    return false;
}

int Database::getUserId(const QString &username)
{
    QSqlQuery query;
    query.prepare("SELECT user_id FROM users WHERE username = ?");
    query.addBindValue(username);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}

int Database::createSection(const QString &name, const QString &abbreviation,
                            const QString &description)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO sections (name, abbreviation, description)
        VALUES (?, ?, ?)
    )");
    query.addBindValue(name);
    query.addBindValue(abbreviation);
    query.addBindValue(description);

    if (!query.exec()) {
        qDebug() << "Error creating section:" << query.lastError();
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool Database::updateSectionBookCount(int sectionId)
{
    QSqlQuery query;
    query.prepare(R"(
        UPDATE sections SET book_count = (
            SELECT COUNT(*) FROM books WHERE section_id = ?
        ) WHERE section_id = ?
    )");
    query.addBindValue(sectionId);
    query.addBindValue(sectionId);

    return query.exec();
}

int Database::createBook(const QString &medium, const QString &title,
                         const QString &author, const QString &genre,
                         int rating, const QString &description,
                         bool read, const QString &viewRights,
                         const QByteArray &cover, int sectionId)
{
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO books (medium, title, author, genre, rating, description,
                          read, view_rights, cover, section_id)
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");
    query.addBindValue(medium);
    query.addBindValue(title);
    query.addBindValue(author);
    query.addBindValue(genre);
    query.addBindValue(rating);
    query.addBindValue(description);
    query.addBindValue(read);
    query.addBindValue(viewRights);
    query.addBindValue(cover);
    query.addBindValue(sectionId);

    if (!query.exec()) {
        qDebug() << "Error creating book:" << query.lastError();
        return -1;
    }

    updateSectionBookCount(sectionId);
    return query.lastInsertId().toInt();
}

bool Database::deleteBook(int bookId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM books WHERE book_id = ?");
    query.addBindValue(bookId);
    return query.exec();
}

bool Database::deleteSection(int sectionId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM sections WHERE section_id = ?");
    query.addBindValue(sectionId);
    return query.exec();
}

bool Database::deleteUser(int userId)
{
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE user_id = ?");
    query.addBindValue(userId);
    return query.exec();
}

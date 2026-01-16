#include "mainwindow.h"
#include "dialogs/newbookdialog.h"
#include "dialogs/newsectiondialog.h"
#include "dialogs/newaccountdialog.h"
#include "dialogs/logindialog.h"
#include "qapplication.h"
#include "ui_mainwindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QTranslator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mCurrentUserName("guest")
    , ui(new Ui::MainWindow)
{
    if (!Database::instance().connect()) {
        QMessageBox::critical(this, "Критична помилка",
                            "Не вдалось підключитись до бази даних!");
        QApplication::quit();
        return;
    }
    else Database::instance().createUser("admin", "123");

    LoginDialog loginDialog(this);
    if (loginDialog.exec() == QDialog::Accepted) {
        mCurrentUserName = loginDialog.username();
        ui->setupUi(this);

        setupUI();
        loadSections();
        loadBooks();

        statusBar()->showMessage(QString("Вітаємо, %1!").arg(mCurrentUserName));
    } else {
        QApplication::quit();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle("Читацький щоденник");
    resize(1200, 700);

    ui->newBookAction->setShortcut(QKeySequence::New);
    connect(ui->newBookAction, &QAction::triggered, this, &MainWindow::onNewBook);

    connect(ui->newSectionAction, &QAction::triggered, this, &MainWindow::onNewSection);

    ui->exitAction->setShortcut(QKeySequence::Quit);
    connect(ui->exitAction, &QAction::triggered, this, &QWidget::close);


    ui->tabs->setTabPosition(QTabWidget::North);
    setCentralWidget(ui->tabs);


    ui->filterLabel->setStyleSheet("font-weight: bold;");
    ui->cbSection->setMinimumWidth(200);
    connect(ui->cbSection, QOverload<int>::of(&QComboBox::currentIndexChanged),this, &MainWindow::onSectionChanged);

    ui->filterLayout->addStretch();


    ui->searchBar->setPlaceholderText("Пошук книг...");

    ui->searchBar->setMaximumWidth(300);
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearch);

    mBooksModel = new QSqlTableModel(this, Database::instance().getDatabase());
    mBooksModel->setTable("books");
    mBooksModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    // Встановлення заголовків
    mBooksModel->setHeaderData(1, Qt::Horizontal, "Носій");
    mBooksModel->setHeaderData(2, Qt::Horizontal, "Назва");
    mBooksModel->setHeaderData(3, Qt::Horizontal, "Автор");
    mBooksModel->setHeaderData(4, Qt::Horizontal, "Жанр");
    mBooksModel->setHeaderData(5, Qt::Horizontal, "Рейтинг");
    mBooksModel->setHeaderData(6, Qt::Horizontal, "Опис");
    mBooksModel->setHeaderData(7, Qt::Horizontal, "Прочитано");
    mBooksModel->setHeaderData(8, Qt::Horizontal, "Права перегляду");
    mBooksModel->select();

    ui->booksTable->setModel(mBooksModel);
    ui->booksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->booksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->booksTable->setAlternatingRowColors(true);
    ui->booksTable->horizontalHeader()->setStretchLastSection(true);
    ui->booksTable->verticalHeader()->hide();
    ui->booksTable->hideColumn(0); // Приховати ID
    ui->booksTable->hideColumn(9); // Приховати обкладинку
    ui->booksTable->hideColumn(10);
    connect(ui->DeleteBook, &QPushButton::clicked, this, &MainWindow::onDeleteBook);

    mSectionsModel = new QSqlTableModel(this, Database::instance().getDatabase());
    mSectionsModel->setTable("sections");
    mSectionsModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    mSectionsModel->setHeaderData(1, Qt::Horizontal, "Назва");
    mSectionsModel->setHeaderData(2, Qt::Horizontal, "Абревіатура");
    mSectionsModel->setHeaderData(3, Qt::Horizontal, "Опис");
    mSectionsModel->setHeaderData(4, Qt::Horizontal, "Шлях");
    mSectionsModel->setHeaderData(5, Qt::Horizontal, "Кількість книг");
    mSectionsModel->setHeaderData(6, Qt::Horizontal, "Групи які мають доступ");
    mSectionsModel->select();


    ui->SectionsTable->setModel(mSectionsModel);
    ui->SectionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->SectionsTable->setAlternatingRowColors(true);
    ui->SectionsTable->horizontalHeader()->setStretchLastSection(true);
    ui->SectionsTable->verticalHeader()->hide();
    ui->SectionsTable->hideColumn(0); // Приховати ID

    connect(ui->DeleteSection, &QPushButton::clicked, this, &MainWindow::onDeleteSection);

    mUsersModel = new QSqlTableModel(this, Database::instance().getDatabase());
    mUsersModel->setTable("users");
    mUsersModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    mUsersModel->setHeaderData(1, Qt::Horizontal, "Користувач");
    mUsersModel->setHeaderData(4, Qt::Horizontal, "Дата створення");
    mUsersModel->setHeaderData(5, Qt::Horizontal, "Останній вхід");
    mUsersModel->select();

    ui->usersTable->setModel(mUsersModel);
    ui->usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->usersTable->setAlternatingRowColors(true);
    ui->usersTable->horizontalHeader()->setStretchLastSection(true);
    ui->usersTable->verticalHeader()->hide();
    ui->usersTable->hideColumn(0); // ID
    ui->usersTable->hideColumn(2); // password_hash
    ui->usersTable->hideColumn(3); // salt

    connect(ui->deleteUser, &QPushButton::clicked, this, &MainWindow::onDeleteUser);
    connect(ui->createUser, &QPushButton::clicked, this, &MainWindow::onNewAccount);

    if(mCurrentUserName != "admin"){
            ui->tabs->tabBar()->setTabVisible(2, false);
    }
    statusBar()->show();

}

void MainWindow::loadSections()
{
    ui->cbSection->clear();
    ui->cbSection->addItem("📚 Всі секції", -1);

    QSqlQuery query("SELECT section_id, name, book_count FROM sections ORDER BY name");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        int bookCount = query.value(2).toInt();

        ui->cbSection->addItem(
            QString("%1 (%2)").arg(name).arg(bookCount), id);
    }
}

void MainWindow::loadBooks(int sectionId)
{
    if (sectionId == -1) {
        mBooksModel->setFilter("");
    } else {
        mBooksModel->setFilter(QString("section_id = %1").arg(sectionId));
    }
    mBooksModel->select();

    statusBar()->showMessage(
        QString("Показано книг: %1").arg(mBooksModel->rowCount()), 3000);
}


void MainWindow::onNewBook()
{
    NewBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        mBooksModel->select();
        mSectionsModel->select();
        loadSections();
        statusBar()->showMessage("Книгу успішно додано!", 3000);
    }
}

void MainWindow::onNewSection()
{
    NewSectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        mSectionsModel->select();
        loadSections();
        statusBar()->showMessage("Секцію успішно додано!", 3000);
    }

}

void MainWindow::onNewAccount()
{
    NewAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        mUsersModel->select();
        statusBar()->showMessage("Акаунт успішно створено!", 3000);
    }
}

void MainWindow::onSectionChanged(int index)
{
    int sectionId = ui->cbSection->itemData(index).toInt();
    loadBooks(sectionId);
}

void MainWindow::onSearch(const QString &text)
{
    if (text.isEmpty()) {
           // Show all books in current section
           if (ui->cbSection->currentIndex() > 0) {
               int sectionId = ui->cbSection->currentData().toInt();
               mBooksModel->setFilter(QString("section_id = %1").arg(sectionId));
           } else {
               mBooksModel->setFilter("");
           }
       } else {
           QString filter = QString("(title LIKE '%%1%' OR author LIKE '%%1%' OR rating LIKE '%%1%' OR genre LIKE '%%1%')").arg(text);

           // Combine with section filter if selected
           if (ui->cbSection->currentIndex() > 0) {
               int sectionId = ui->cbSection->currentData().toInt();
               filter = QString("section_id = %1 AND %2").arg(sectionId).arg(filter);
           }

           mBooksModel->setFilter(filter);
       }
       mBooksModel->select();
}


void MainWindow::onDeleteBook()
{
    QModelIndex index = ui->booksTable->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Помилка", "Оберіть книгу для видалення");
        return;
    }

    int row = index.row();
    int bookId = mBooksModel->record(row).value("book_id").toInt();
    QString title = mBooksModel->record(row).value("title").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Підтвердження",
        QString("Ви впевнені, що хочете видалити книгу '%1'?").arg(title),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteBook(bookId)) {
            QMessageBox::information(this, "Успіх", "Книгу видалено!");
            mBooksModel->select();
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалось видалити книгу");
        }
    }
    loadBooks();
}

void MainWindow::onDeleteSection()
{
    QModelIndex index = ui->SectionsTable->currentIndex();
    if(!index.isValid()){
        QMessageBox::warning(this, "Помилка", "Оберіть секцію для видалення");
        return;
    }

    int row = index.row();
    int sectionId = mSectionsModel->record(row).value("section_id").toInt();
    QString name = mSectionsModel->record(row).value("name").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Підтвердження",
        QString("Ви впевнені, що хочете видалити секцію '%1'?").arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteSection(sectionId)) {
            QMessageBox::information(this, "Успіх", "Секцію видалено!");
            mSectionsModel->select();
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалось видалити книгу");
        }
    }
    void loadSections();
}

void MainWindow::onDeleteUser()
{
    QModelIndex index = ui->usersTable->currentIndex();
    if(!index.isValid()){
        QMessageBox::warning(this, "Помилка", "Оберіть користувача для видалення");
        return;
    }

    int row = index.row();
    int userId = mUsersModel->record(row).value("user_id").toInt();
    QString userName = mUsersModel->record(row).value("username").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Підтвердження",
        QString("Ви впевнені, що хочете видалити користувача '%1'?").arg(userName),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteUser(userId)) {
            QMessageBox::information(this, "Успіх", "Користувача видалено!");
            mUsersModel->select();
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалось видалити Користувача");
        }
    }
}

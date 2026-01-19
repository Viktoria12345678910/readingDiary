#include "mainwindow.h"
#include "dialogs/newbookdialog.h"
#include "dialogs/newsectiondialog.h"
#include "dialogs/newaccountdialog.h"
#include "dialogs/logindialog.h"
#include "dialogs/changepassworddialog.h"
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
#include <QSettings>
#include <QTranslator>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mCurrentUserName("guest")
    , ui(new Ui::MainWindow)
{


    LoginDialog loginDialog(this);
    if (loginDialog.exec() == QDialog::Accepted) {
        mCurrentUserName = loginDialog.username();
        ui->setupUi(this);

        setupUI();
        loadSections();
        loadBooks();

        statusBar()->showMessage(QString(tr("Вітаємо, %1!")).arg(mCurrentUserName));
    } else {
        QApplication::quit();
    }

    // mInstance = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    setWindowTitle(tr("Читацький щоденник"));
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


    ui->searchBar->setPlaceholderText(tr("Пошук книг..."));

    ui->searchBar->setMaximumWidth(300);
    connect(ui->searchBar, &QLineEdit::textChanged, this, &MainWindow::onSearch);

    mBooksModel = new QSqlTableModel(this, Database::instance().getDatabase());
    mBooksModel->setTable("books");
    mBooksModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    // Встановлення заголовків
    mBooksModel->setHeaderData(1, Qt::Horizontal, tr("Носій"));
    mBooksModel->setHeaderData(2, Qt::Horizontal, tr("Назва"));
    mBooksModel->setHeaderData(3, Qt::Horizontal, tr("Автор"));
    mBooksModel->setHeaderData(4, Qt::Horizontal, tr("Жанр"));
    mBooksModel->setHeaderData(5, Qt::Horizontal, tr("Рейтинг"));
    mBooksModel->setHeaderData(6, Qt::Horizontal, tr("Опис"));
    mBooksModel->setHeaderData(7, Qt::Horizontal, tr("Прочитано"));
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
    ui->booksTable->hideColumn(8);
    connect(ui->DeleteBook, &QPushButton::clicked, this, &MainWindow::onDeleteBook);

    mSectionsModel = new QSqlTableModel(this, Database::instance().getDatabase());
    mSectionsModel->setTable("sections");
    mSectionsModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    mSectionsModel->setHeaderData(1, Qt::Horizontal, tr("Назва"));
    mSectionsModel->setHeaderData(2, Qt::Horizontal, tr("Абревіатура"));
    mSectionsModel->setHeaderData(3, Qt::Horizontal, tr("Опис"));
    mSectionsModel->setHeaderData(4, Qt::Horizontal, tr("Шлях"));
    mSectionsModel->setHeaderData(5, Qt::Horizontal, tr("Кількість книг"));
    mSectionsModel->select();


    ui->SectionsTable->setModel(mSectionsModel);
    ui->SectionsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->SectionsTable->setAlternatingRowColors(true);
    ui->SectionsTable->horizontalHeader()->setStretchLastSection(true);
    ui->SectionsTable->verticalHeader()->hide();
    ui->SectionsTable->hideColumn(0); // Приховати ID
    ui->SectionsTable->hideColumn(6);

    connect(ui->DeleteSection, &QPushButton::clicked, this, &MainWindow::onDeleteSection);

    mUsersModel = new QSqlTableModel(this, Database::instance().getDatabase());
    mUsersModel->setTable("users");
    mUsersModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    mUsersModel->setHeaderData(1, Qt::Horizontal, tr("Користувач"));
    mUsersModel->setHeaderData(4, Qt::Horizontal, tr("Дата створення"));
    mUsersModel->setHeaderData(5, Qt::Horizontal, tr("Останній вхід"));
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
    ui->cbSection->addItem(tr("📚 Всі секції"), -1);

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
        QString(tr("Показано книг: %1")).arg(mBooksModel->rowCount()), 3000);
}


void MainWindow::onNewBook()
{
    NewBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        mBooksModel->select();
        mSectionsModel->select();
        loadSections();
        statusBar()->showMessage(tr("Книгу успішно додано!"), 3000);
    }
}

void MainWindow::onNewSection()
{
    NewSectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        mSectionsModel->select();
        loadSections();
        statusBar()->showMessage(tr("Секцію успішно додано!"), 3000);
    }

}

void MainWindow::onNewAccount()
{
    NewAccountDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        mUsersModel->select();
        statusBar()->showMessage(tr("Акаунт успішно створено!"), 3000);
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
        QMessageBox::warning(this, tr("Помилка"), tr("Оберіть книгу для видалення"));
        return;
    }

    int row = index.row();
    int bookId = mBooksModel->record(row).value("book_id").toInt();
    QString title = mBooksModel->record(row).value("title").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Підтвердження"),
        QString(tr("Ви впевнені, що хочете видалити книгу '%1'?")).arg(title),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteBook(bookId)) {
            QMessageBox::information(this, tr("Успіх"), tr("Книгу видалено!"));
            mBooksModel->select();
        } else {
            QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось видалити книгу"));
        }
    }
    loadBooks();
}

void MainWindow::onDeleteSection()
{
    QModelIndex index = ui->SectionsTable->currentIndex();
    if(!index.isValid()){
        QMessageBox::warning(this, tr("Помилка"), tr("Оберіть секцію для видалення"));
        return;
    }

    int row = index.row();
    int sectionId = mSectionsModel->record(row).value("section_id").toInt();
    QString name = mSectionsModel->record(row).value("name").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Підтвердження"),
        QString(tr("Ви впевнені, що хочете видалити секцію '%1'?")).arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteSection(sectionId)) {
            QMessageBox::information(this, tr("Успіх"), tr("Секцію видалено!"));
            mSectionsModel->select();
        } else {
            QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось видалити книгу"));
        }
    }
    void loadSections();
}

void MainWindow::onDeleteUser()
{
    QModelIndex index = ui->usersTable->currentIndex();
    if(!index.isValid()){
        QMessageBox::warning(this, tr("Помилка"), tr("Оберіть користувача для видалення"));
        return;
    }

    int row = index.row();
    int userId = mUsersModel->record(row).value("user_id").toInt();
    QString userName = mUsersModel->record(row).value("username").toString();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, tr("Підтвердження"),
        QString(tr("Ви впевнені, що хочете видалити користувача '%1'?")).arg(userName),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Database::instance().deleteUser(userId)) {
            QMessageBox::information(this, tr("Успіх"), tr("Користувача видалено!"));
            mUsersModel->select();
        } else {
            QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось видалити Користувача"));
        }
    }
}

QTranslator& MainWindow::getTranslator(){
    return *mTranslator;
}

void MainWindow::on_pbChangeLang_clicked()
{
    QString lang =ui->cbLanguage->currentText();
    QSettings settings;
    settings.setValue("langeuage", lang);
    QString path = ":/translations/"+lang;
    qDebug() <<"translation... "<<path;
    if(mTranslator->load(path))
        QCoreApplication::installTranslator(mTranslator);
    ui->retranslateUi(this);
}

void MainWindow::setTranslator(QTranslator* translator) {
    mTranslator = translator;
}


void MainWindow::on_pbChangePassword_clicked()
{
        qDebug() <<"it works so far";
    QModelIndex index = ui->usersTable->currentIndex();
    if(!index.isValid()){
        QMessageBox::warning(this, tr("Помилка"), tr("Оберіть користувача для зміни паролю!"));
        return;
    }
    qDebug() <<"it works so far";
    int row = index.row();
    // int userId = mUsersModel->record(row).value("user_id").toInt();
    QString username = mUsersModel->record(row).value("username").toString();
    qDebug() <<"it works so far";
    ChangePasswordDialog dialog(this);
    qDebug() <<"it works so far init dialog";
    dialog.setUsername(username);
    if (dialog.exec() == QDialog::Accepted) {
        statusBar()->showMessage(tr("Пароль успішно змінено!"), 3000);
    }


}


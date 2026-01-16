#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTableView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSqlTableModel>
#include <QTextBrowser>
#include <QComboBox>
#include "database.h"
#include <QMessageBox>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNewBook();
    void onNewSection();
    void onNewAccount();
    void onSectionChanged(int index);
    void onSearch(const QString &text);
    void onDeleteBook();
    void onDeleteSection();
    void onDeleteUser();

private:
    void setupUI();
    void loadSections();
    void loadBooks(int sectionId = -1);
    void loadUsers();

    QTabWidget *mTabWidget;
    QTableView *mBooksTable;
    QTableView *mSectionsTable;
    QTableView *mUsersTable;
    QTextBrowser *mHelp;
    QSqlTableModel *mBooksModel;
    QSqlTableModel *mSectionsModel;
    QSqlTableModel *mUsersModel;
    QComboBox *mSectionFilter;
    QLineEdit *mSearchBar;
    QPushButton *mDeleteBook;
    QPushButton *mDeleteSection;
    QPushButton *mDeleteUser;
    QPushButton *mCreateUser;


    QString mCurrentUserName;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

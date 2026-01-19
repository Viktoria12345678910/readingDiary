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
#include <QTranslator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTranslator& getTranslator();
    void setTranslator(QTranslator* translator);


private slots:
    void onNewBook();
    void onNewSection();
    void onNewAccount();
    void onSectionChanged(int index);
    void onSearch(const QString &text);
    void onDeleteBook();
    void onDeleteSection();
    void onDeleteUser();

    void on_pbChangeLang_clicked();

    void on_pbChangePassword_clicked();

private:
    void setupUI();
    void loadSections();
    void loadBooks(int sectionId = -1);
    void loadUsers();

    QSqlTableModel *mBooksModel;
    QSqlTableModel *mSectionsModel;
    QSqlTableModel *mUsersModel;
    QTranslator *mTranslator = nullptr;
    static MainWindow* mInstance;

    QString mCurrentUserName;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

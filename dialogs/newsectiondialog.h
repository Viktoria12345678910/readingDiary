#ifndef NEWSECTIONDIALOG_H
#define NEWSECTIONDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>


class NewSectionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewSectionDialog(QWidget *parent = nullptr);
public slots:
    void onSave();
private:
    void setupUI();

    QLineEdit *mName;
    QLineEdit *mAbbreviation;
    QTextEdit *mDescription;
    QLineEdit *mBookCount;
    QLineEdit *mAccessGroup;
};

#endif // NEWSECTIONDIALOG_H

#ifndef NEWACCOUNTDIALOG_H
#define NEWACCOUNTDIALOG_H

#include <QDialog>
#include <QLineEdit>

class NewAccountDialog : public QDialog
{
    Q_OBJECT
public:
    explicit NewAccountDialog(QWidget *parent = nullptr);
private slots:
    void onSave();
private:
    void setupUI();

    QLineEdit *m_usernameEdit;
    QLineEdit *m_passwordEdit;
    QLineEdit *m_confirmPasswordEdit;
};

#endif // NEWACCOUNTDIALOG_H

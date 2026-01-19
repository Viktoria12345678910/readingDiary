#ifndef CHANGEPASSWORDDIALOG_H
#define CHANGEPASSWORDDIALOG_H

#include <QDialog>
#include <QLineEdit>

class ChangePasswordDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChangePasswordDialog(QWidget *parent = nullptr);
    void setUsername(QString username);
private slots:
    void onSave();
private:
    void setupUI();
    QLineEdit *mNewPassword;
    QLineEdit *mConfirmPassword;
    QLineEdit *mAdminPassword;
    QString mUsername;
};

#endif // CHANGEPASSWORDDIALOG_H

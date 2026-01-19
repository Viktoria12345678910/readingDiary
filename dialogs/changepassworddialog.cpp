#include "changepassworddialog.h"
#include "../database.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QDebug>


ChangePasswordDialog::ChangePasswordDialog(QWidget *parent)
    :QDialog(parent)
{
    setupUI();
}

void ChangePasswordDialog::setupUI()
{
    QString title = QString(tr("Зміна паролю для '%1'")).arg(mUsername);
    setWindowTitle(title);
    setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    mNewPassword = new QLineEdit;
    mNewPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow(tr("Новий пароль*:"), mNewPassword);

    mConfirmPassword = new QLineEdit;
    mConfirmPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow(tr("Підтвердження паролю*:"), mConfirmPassword);

    mAdminPassword = new QLineEdit;
    mAdminPassword->setEchoMode(QLineEdit::Password);
    formLayout->addRow(tr("Адміністраторський пароль*:"), mAdminPassword);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &ChangePasswordDialog::onSave);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void ChangePasswordDialog::onSave()
{
    QString AdmPassword = mAdminPassword->text();
    QString username = mUsername;
    QString newPassword;
    int userId = Database::instance().getUserId(username);
    if (mNewPassword->text().isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Введіть новий пароль!"));
        return;
    }

    if (mConfirmPassword->text() != mNewPassword->text()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Паролі не співпадають!"));
        return;
    }
    else
        newPassword = mNewPassword->text();

    if (!Database::instance().authenticateUser("admin", AdmPassword)) {
        QMessageBox::warning(this, tr("Помилка"), tr("Неправильний пароль адміністратора!"));
        return;
    }
    QString msg = QString(tr("Пароль було успішно змінено на '%1'")).arg(newPassword);

    if(Database::instance().changePassword(userId, newPassword)){
        QMessageBox::information(this, tr("Успіх"), msg);
        accept();
    }
    else
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось змінити пароль!"));

}

void ChangePasswordDialog::setUsername(QString username){
    mUsername = username;
    qDebug() <<" it works so far "<<mUsername;
}

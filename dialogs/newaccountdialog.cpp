#include "newaccountdialog.h"
#include "../database.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>



NewAccountDialog::NewAccountDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
}

void NewAccountDialog::setupUI()
{
    setWindowTitle(tr("Створити новий акаунт"));
    setMinimumWidth(400);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    m_usernameEdit = new QLineEdit();
    formLayout->addRow(tr("Ім'я користувача*:"), m_usernameEdit);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow(tr("Пароль*:"), m_passwordEdit);

    m_confirmPasswordEdit = new QLineEdit();
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow(tr("Підтвердження паролю*:"), m_confirmPasswordEdit);

    mainLayout->addLayout(formLayout);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewAccountDialog::onSave);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}

void NewAccountDialog::onSave()
{
    if (m_usernameEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Ім'я користувача обов'язкове!"));
        return;
    }

    if (m_passwordEdit->text().isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Пароль обов'язковий!"));
        return;
    }

    if (m_passwordEdit->text() != m_confirmPasswordEdit->text()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Паролі не співпадають!"));
        return;
    }

    if (Database::instance().createUser(m_usernameEdit->text(), m_passwordEdit->text())) {
        QMessageBox::information(this, tr("Успіх"), tr("Акаунт успішно створено!"));
        accept();
    } else {
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось створити акаунт!"));
    }
}

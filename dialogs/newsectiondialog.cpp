#include "newsectiondialog.h"
#include "../database.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QPushButton>

NewSectionDialog::NewSectionDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
}

void NewSectionDialog::setupUI()
{
    setWindowTitle(tr("Додати нову секцію"));
    setMinimumWidth(500);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    // Назва
    mName = new QLineEdit();
    formLayout->addRow(tr("Назва*:"), mName);

    // Скорочення
    mAbbreviation = new QLineEdit();
    formLayout->addRow(tr("Скорочення:"), mAbbreviation);

    // Опис
    mDescription = new QTextEdit();
    mDescription->setMaximumHeight(100);
    formLayout->addRow(tr("Опис:"), mDescription);

    // Кількість книг
    mBookCount = new QLineEdit();
    mBookCount->setReadOnly(true);
    mBookCount->setText("0");
    formLayout->addRow(tr("Кількість книг:"), mBookCount);

    mainLayout->addLayout(formLayout);

    // Кнопки
    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &NewSectionDialog::onSave);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    mainLayout->addWidget(buttonBox);
}


void NewSectionDialog::onSave()
{
    if (mName->text().isEmpty()) {
        QMessageBox::warning(this, tr("Помилка"), tr("Назва секції обов'язкова!"));
        return;
    }

    QString name = mName->text();
    QString abbreviation = mAbbreviation->text();
    QString description = mDescription->toPlainText();

    int sectionId = Database::instance().createSection(
        name, abbreviation, description);

    if (sectionId > 0) {
        QMessageBox::information(this, tr("Успіх"), tr("Секцію успішно додано!"));
        accept();
    } else {
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалось додати секцію!"));
    }
}

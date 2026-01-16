#include "sectionmanagerdialog.h"
#include "../database.h"
#include "dialogs/newsectiondialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QFormLayout>



SectionManagerDialog::SectionManagerDialog(QWidget *parent)
    :QDialog(parent)
{
    setupUI();
    refreshSectionList();
}

void SectionManagerDialog::setupUI()
{
    setWindowTitle("Управління секціями");
    setMinimumSize(400, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mSectionList = new QListWidget;
    mainLayout->addWidget(mSectionList);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    mCreateButton = new QPushButton("Створити секцію");
    connect(mCreateButton, &QPushButton::clicked, this, &SectionManagerDialog::onCreateSection);
    buttonLayout->addWidget(mCreateButton);

    mDeleteButton = new QPushButton("Видалити секцію");
    connect(mDeleteButton, &QPushButton::clicked, this, &SectionManagerDialog::onDeleteSection);
    buttonLayout->addWidget(mDeleteButton);

    mainLayout->addLayout(buttonLayout);

    QPushButton *closeButton = new QPushButton("Закрити");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);

}

void SectionManagerDialog::onCreateSection()
{
    NewSectionDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        loadSections();
    }
}

void SectionManagerDialog::onDeleteSection(){
    QListWidgetItem *item = mSectionList->currentItem();
    if(!item){
        QMessageBox::warning(this, "Попередження","Оберіть секцію для видалення");
        return;
    }

    int sectionId = item->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Підтвердження", "Ви впевнені що хочете видалити секцію?", QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::Yes){
        QSqlQuery query;
        query.prepare("DELETE FROM sections WHERE section_id = ?");
        query.addBindValue(sectionId);
        if (query.exec()){
            QMessageBox::information(this, "Успіх","Секцію видалено!");
            refreshSectionList();
        } else {
            QMessageBox::critical(this, "Помилка", "Не вдалось видалити секцію");
        }
    }
}

void SectionManagerDialog::refreshSectionList()
{
    mSectionList->clear();

    QSqlQuery query("SELECT section_id, name, description, book_count, access_group FROM sections");
    while(query.next()){
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();

        QListWidgetItem *item = new QListWidgetItem(name);
        item->setData(Qt::UserRole, id);
        mSectionList->addItem(item);
    }
}

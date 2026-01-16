#ifndef SECTIONMANAGERDIALOG_H
#define SECTIONMANAGERDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLineEdit>


class SectionManagerDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SectionManagerDialog(QWidget *parent = nullptr);
public slots:
    void onCreateSection();
    void onDeleteSection();
    void refreshSectionList();

private:
    void setupUI();

    QListWidget *mSectionList;
    QPushButton *mCreateButton;
    QPushButton *mDeleteButton;
};

#endif // SECTIONMANAGERDIALOG_H

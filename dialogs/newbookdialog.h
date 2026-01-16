#ifndef NEWBOOKDIALOG_H
#define NEWBOOKDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>

class NewBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewBookDialog(QWidget *parent = nullptr);

private slots:
    void onSave();
    void onSelectCover();

private:
    void setupUI();
    void loadSections();

    QComboBox *mMediumCombo;
    QLineEdit *mTitleEdit;
    QLineEdit *mAuthorEdit;
    QLineEdit *mGenreEdit;
    QSpinBox *mRatingSpin;
    QTextEdit *mDescriptionEdit;
    QCheckBox *mReadCheck;
    QComboBox *mViewRightsCombo;
    QComboBox *mSectionCombo;
    QPushButton *mCoverButton;
    QLabel *mCoverPreview;

    QByteArray mCoverData;
};

#endif // NEWBOOKDIALOG_H

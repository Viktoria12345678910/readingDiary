#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTranslator>
#include <QSettings>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (!Database::instance().connect()) {
        qDebug() <<" connection to db failed";
        QApplication::quit();
        return 1;
    }
    QFile styleFile(":/style.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            QString style = QLatin1String(styleFile.readAll());
            a.setStyleSheet(style);
            styleFile.close();
        }
    QCoreApplication::setOrganizationName("Viktoria");
    QCoreApplication::setApplicationName("readingDiary");
    QSettings settings;
    if(!settings.contains("language"))
        settings.setValue("language", "English");

    QTranslator translator;
    QString lang = settings.value("language").toString();
    QString path = ":/translations/" + lang + ".qm";
    qDebug() <<"translation "<<path;
    qDebug() << settings.fileName();
    if(translator.load(path)){
        qDebug() <<"installing translator";
        a.installTranslator(&translator);
        qDebug() <<"translation loaded ok";
    }
    else qDebug() <<"translation didnt load";

    MainWindow w;
    w.setTranslator(&translator);
    w.show();
    return a.exec();
}

#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile styleFile(":/style.qss");
        if (styleFile.open(QFile::ReadOnly)) {
            QString style = QLatin1String(styleFile.readAll());
            a.setStyleSheet(style);
            styleFile.close();
        }
    QTranslator translator;
    if(translator.load("eng.qm"))
        a.installTranslator(&translator);


    MainWindow w;
    w.show();
    return a.exec();
}

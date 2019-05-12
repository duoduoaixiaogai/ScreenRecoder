#pragma warning(disable: 4819)

#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator transZh;
    if (!transZh.load("ScreenRecoder_zh.qm")) {
        QMessageBox::warning(nullptr, "", QCoreApplication::translate("main", "Error load translator file!"));
    } else {
        a.installTranslator(&transZh);
    }

    MainWindow w;
    w.show();

    return a.exec();
}

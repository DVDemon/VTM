#include "mainwindow.h"
#include <QApplication>
#include "configuration.h"
#include <QDebug>
#include <VMTLine.h>
#include <QSplashScreen>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("Virtual Turing Machine");
    QApplication::setOrganizationDomain("mai.ru");

    a.setWindowIcon(QIcon(":/images/app.icns"));
    Configuration::GetInstance().Init(&a);
    MainWindow w;
    w.show();

    return a.exec();
}

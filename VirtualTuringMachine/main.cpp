#include "mainwindow.h"
#include "vmttheme.h"
#include <QApplication>
#include <QIcon>
#include "configuration.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName("Virtual Turing Machine");
    QApplication::setOrganizationDomain("mai.ru");

    VmtTheme::applyApplication(&a);

    a.setWindowIcon(QIcon(":/images/app.icns"));
    Configuration::GetInstance().Init(&a);
    MainWindow w;
    w.show();

    return a.exec();
}

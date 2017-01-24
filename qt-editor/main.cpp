#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(application);

    QApplication a(argc, argv);
    a.setOrganizationName("TrollTech");
    a.setApplicationName("Application Example");

    MainWindow w;

#if defined(Q_OS_SYMBIAN)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}

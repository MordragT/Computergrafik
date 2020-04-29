#include "mainwindow.h"

#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;

    // OpenGL Version
    format.setVersion(3, 3);

    //OpenGL Profile(only Core)
    format.setProfile(QSurfaceFormat::CoreProfile);

    //MacOs fix
    QSurfaceFormat::setDefaultFormat(format);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char** argv)
{
    QGuiApplication::setDesktopSettingsAware(false);
    QGuiApplication::setAttribute(Qt::AA_ForceRasterWidgets, true);
    QApplication app(argc, argv);
    bean::mainwindow mainwin;
    mainwin.show();
    return app.exec();
}

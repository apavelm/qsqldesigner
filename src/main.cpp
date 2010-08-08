#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Company name");
    app.setApplicationName("SQL Designer");

    QSplashScreen splash(QPixmap(":/splash"));
    splash.showMessage(QSplashScreen::tr("Loading..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    splash.show();
    app.processEvents();

    MainWindow w;
    w.showMaximized();
    splash.finish(&w);
    return app.exec();
}

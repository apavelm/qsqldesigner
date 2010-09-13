#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include "mainwindow.h"
#include "settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SettingsManager::newInstance();
    app.setOrganizationName("Company name");
    app.setApplicationName("SQL Designer");

    QSplashScreen splash(QPixmap(":/splash"));
    splash.showMessage(QSplashScreen::tr("Loading..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    splash.show();
    app.processEvents();

    MainWindow w;
    w.setWindowTitle(app.applicationName());
    w.showMaximized();
    splash.finish(&w);
    int rslt = app.exec();
    SettingsManager::deleteInstance();
    return rslt;
}

#include <QtCore/QScopedPointer>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include "mainwindow.h"
#include "models/modelmanager.h"
#include "settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SettingsManager::newInstance();
    app.setOrganizationName("Company name");
    app.setApplicationName("SQL Designer");

    QScopedPointer<QSplashScreen> splash(new QSplashScreen(QPixmap(":/splash")));
    splash->showMessage(QSplashScreen::tr("Loading..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    splash->showMessage(QSplashScreen::tr("Starting Model Manager..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    ModelManager::newInstance();
    splash->show();
    app.processEvents();

    MainWindow w;
    w.setWindowTitle(app.applicationName());
    w.showMaximized();
    splash->finish(&w);
    int rslt = app.exec();
    ModelManager::deleteInstance();
    SettingsManager::deleteInstance();
    return rslt;
}

#include <QtCore/QScopedPointer>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include "mainwindow.h"
#include "projectmanager.h"
#include "pluginmanager.h"
#include "settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("Company name");
    app.setApplicationName("SQL Designer");

    QScopedPointer<QSplashScreen> splash(new QSplashScreen(QPixmap(":/splash")));
    splash->show();
    splash->showMessage(QSplashScreen::tr("Loading..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    splash->showMessage(QSplashScreen::tr("Starting Settings Manager..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    SettingsManager::newInstance();
    splash->showMessage(QSplashScreen::tr("Settings Manager: loading saved settings"), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    //SettingsManager::getInstance()->loadSettings(); // TODO: !!!!!!!!
    splash->showMessage(QSplashScreen::tr("Starting Plugin Manager..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    PluginManager::newInstance();
    splash->showMessage(QSplashScreen::tr("Plugin Manager: loading plugins"), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    PluginManager::getInstance()->loadDataTypePlugins();
    splash->showMessage(QSplashScreen::tr("Starting Project Manager..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    ProjectManager::newInstance();

    MainWindow w;
    w.setWindowTitle(app.applicationName());
    w.showMaximized();
    splash->finish(&w);
    int rslt = app.exec();

    ProjectManager::deleteInstance();
    PluginManager::deleteInstance();
    SettingsManager::deleteInstance();
    return rslt;
}
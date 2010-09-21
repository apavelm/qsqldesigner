#include <QtCore/QScopedPointer>
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include "mainwindow.h"
#include "models/modelmanager.h"
#include "widgets/widgetmanager.h"
#include "settingsmanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    SettingsManager::newInstance();
    app.setOrganizationName("Company name");
    app.setApplicationName("SQL Designer");

    QScopedPointer<QSplashScreen> splash(new QSplashScreen(QPixmap(":/splash")));
    splash->show();
    splash->showMessage(QSplashScreen::tr("Loading..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    splash->showMessage(QSplashScreen::tr("Starting Model Manager..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    app.processEvents();
    ModelManager::newInstance();
    splash->showMessage(QSplashScreen::tr("Starting Widget Manager..."), Qt::AlignBottom | Qt::AlignHCenter, Qt::black);
    WidgetManager::newInstance();
    app.processEvents();

    MainWindow w;
    w.setWindowTitle(app.applicationName());
    w.showMaximized();
    splash->finish(&w);
    int rslt = app.exec();
    WidgetManager::deleteInstance();
    ModelManager::deleteInstance();
    SettingsManager::deleteInstance();
    return rslt;
}

/***************************************************************************
 *   Copyright (C) 2010 by Pavel Andreev                                   *
 *   Mail: apavelm on gmail point com (apavelm@gmail.com)                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, you can read it                      *
 *   here <http://www.gnu.org/licenses/>, or write to the                  *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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
    PluginManager::getInstance()->loadDBMSPlugins();
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

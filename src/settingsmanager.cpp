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

#include "settingsmanager.h"

#include <QtCore/QCoreApplication>

SettingsManager::SettingsManager() : m_fontPointSize(8), m_smallPointSize(6)
{
    // set default values;
    m_paperColor = QColor(170, 185, 220);
    m_tableColorStart = QColor(229, 219, 204);
    m_tableColorStop = QColor(252, 242, 227);
    m_fontColor = QColor(0, 0, 0);
    m_columnFontColor = QColor(Qt::darkRed);
    m_mainFont = QFont("Arial", m_fontPointSize);
    m_smallFont = QFont("Arial", m_smallPointSize);
    m_penWidth = 2.0;
    m_gridSize = 20.0;
    m_arrowSize = 10;
    m_showGrid = true;
    m_antialiasing = false;
    m_textAntialiasing = true;
    m_smoothPixmapTransform = false;
    m_highQualityAntialiasing = false;
    m_nonCosmeticDefaultPen = false;
    m_pluginDirectory = qApp->applicationDirPath() + "/plugins/";
}

SettingsManager::~SettingsManager()
{
}

void SettingsManager::setShowGrid(bool showGrid)
{
    m_showGrid = showGrid;
    emit dirty();
}

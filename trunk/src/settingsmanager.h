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

#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QtGui/QColor>
#include <QtGui/QFont>

#include "singleton.h"

class SettingsManager: public QObject, public Singleton<SettingsManager>
{
    Q_OBJECT
public:
    inline const QColor& paperColor() const {return m_paperColor;}
    inline const QColor& tableColorStart() const {return m_tableColorStart;}
    inline const QColor& tableColorStop() const {return m_tableColorStop;}
    inline const QColor& fontColor() const {return m_fontColor;}
    inline const QColor& columnFontColor() const {return m_columnFontColor;}
    inline const QFont&  mainFont() const {return m_mainFont;}
    inline const QFont&  smallFont() const {return m_smallFont;}
    inline int  fontPointSize() const {return m_fontPointSize;}
    inline int  smallFontPointSize() const {return m_smallPointSize;}
    inline qreal  penWidth() const {return m_penWidth;}
    inline qreal  gridSize() const {return m_gridSize;}
    inline bool showGrid() const {return m_showGrid;}
    inline bool antialiasing() const {return m_antialiasing;}
    inline bool textAntialiasing() const {return m_textAntialiasing;}
    inline bool smoothPixmapTransform() const {return m_smoothPixmapTransform;}
    inline bool highQualityAntialiasing() const {return m_highQualityAntialiasing;}
    inline bool nonCosmeticDefaultPen() const {return m_nonCosmeticDefaultPen;}
    inline const QString& pluginDirectory() const {return m_pluginDirectory;}
public slots:
    inline void setPaperColor(const QColor& color) {m_paperColor = color;}
    inline void setTableColorStart(const QColor& color) {m_tableColorStart = color;}
    inline void setTableColorStop(const QColor& color) {m_tableColorStop = color;}
    inline void setFontColor(const QColor& color) {m_fontColor = color;}
    inline void setColumnFontColor(const QColor& color) {m_columnFontColor = color;}
    inline void setMainFont(const QFont& font) {m_mainFont = font;}
    inline void setSmallFont(const QFont& font) {m_smallFont = font;}
    inline void setFontPointSize(int newSize) {m_fontPointSize = newSize;}
    inline void setSmallFontPointSize(int newSize) {m_smallPointSize = newSize;}
    inline void setPenWidth(qreal newWidth) {m_penWidth = newWidth;}
    inline void setGridSize(qreal newGridSize) {m_gridSize = newGridSize;}
    void setShowGrid(bool showGrid);
    inline void setAntialiasing(bool antialiasing) {m_antialiasing = antialiasing;}
    inline void setTextAntialiasing(bool textAntialiasing) {m_textAntialiasing = textAntialiasing;}
    inline void setSmoothPixmapTransform(bool smoothPixmapTransform) {m_smoothPixmapTransform = smoothPixmapTransform;}
    inline void setHighQualityAntialiasing(bool highQualityAntialiasing) {m_highQualityAntialiasing = highQualityAntialiasing;}
    inline void setNonCosmeticDefaultPen(bool nonCosmeticDefaultPen) {m_nonCosmeticDefaultPen = nonCosmeticDefaultPen;}
    inline void setPluginDirectory(const QString& path) {m_pluginDirectory = path;}
private:
    friend class Singleton<SettingsManager>;
    SettingsManager();
    virtual ~SettingsManager();

    //members
    QColor m_paperColor;
    QColor m_tableColorStart;
    QColor m_tableColorStop;
    QColor m_fontColor;
    QColor m_columnFontColor;
    QFont  m_mainFont;
    QFont  m_smallFont;
    int    m_fontPointSize;
    int    m_smallPointSize;
    qreal  m_penWidth;
    qreal  m_gridSize;
    bool   m_showGrid;
    bool   m_antialiasing;
    bool   m_textAntialiasing;
    bool   m_smoothPixmapTransform;
    bool   m_highQualityAntialiasing;
    bool   m_nonCosmeticDefaultPen;
    QString m_pluginDirectory;
signals:
    void dirty();
};

#define SM SettingsManager::getInstance()

#endif // SETTINGSMANAGER_H

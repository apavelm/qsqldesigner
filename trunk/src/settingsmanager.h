#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QtGui/QColor>
#include <QtGui/QFont>

#include "singleton.h"

class SettingsManager: public Singleton<SettingsManager>
{
public:
    inline const QColor& paperColor() const {return m_paperColor;}
    inline void setPaperColor(const QColor& color) {m_paperColor = color;}

    inline const QColor& tableColorStart() const {return m_tableColorStart;}
    inline void setTableColorStart(const QColor& color) {m_tableColorStart = color;}
    inline const QColor& tableColorStop() const {return m_tableColorStop;}
    inline void setTableColorStop(const QColor& color) {m_tableColorStop = color;}

    inline const QColor& fontColor() const {return m_fontColor;}
    inline void setFontColor(const QColor& color) {m_fontColor = color;}

    inline const QColor& columnFontColor() const {return m_columnFontColor;}
    inline void setColumnFontColor(const QColor& color) {m_columnFontColor = color;}

    inline const QFont&  mainFont() const {return m_mainFont;}
    inline void setMainFont(const QFont& font) {m_mainFont = font;}

    inline const QFont&  smallFont() const {return m_smallFont;}
    inline void setSmallFont(const QFont& font) {m_smallFont = font;}

    inline int  fontPointSize() const {return m_fontPointSize;}
    inline void setFontPointSize(int newSize) {m_fontPointSize = newSize;}

    inline int  smallFontPointSize() const {return m_smallPointSize;}
    inline void setSmallFontPointSize(int newSize) {m_smallPointSize = newSize;}

    inline qreal  penWidth() const {return m_penWidth;}
    inline void setPenWidth(qreal newWidth) {m_penWidth = newWidth;}

    inline int  gridSize() const {return m_gridSize;}
    inline void setGridSize(int newGridSize) {m_gridSize = newGridSize;}
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
    int    m_gridSize;
};

#define SM SettingsManager::getInstance()

#endif // SETTINGSMANAGER_H

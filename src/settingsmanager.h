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
    inline int  gridSize() const {return m_gridSize;}
    inline bool showGrid() const {return m_showGrid;}
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
    inline void setGridSize(int newGridSize) {m_gridSize = newGridSize;}
    inline void setShowGrid(bool showGrid) {m_showGrid = showGrid;}
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
    bool   m_showGrid;
signals:
    void dirty();
};

#define SM SettingsManager::getInstance()

#endif // SETTINGSMANAGER_H

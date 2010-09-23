#include "settingsmanager.h"

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
    m_showGrid = true;
    m_antialiasing = false;
    m_textAntialiasing = true;
    m_smoothPixmapTransform = false;
    m_highQualityAntialiasing = false;
    m_nonCosmeticDefaultPen = false;
}

SettingsManager::~SettingsManager()
{
}

void SettingsManager::setShowGrid(bool showGrid)
{
    m_showGrid = showGrid;
    emit dirty();
}

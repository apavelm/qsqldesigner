#include "settingsmanager.h"

SettingsManager::SettingsManager()
{
    // set default values;
    m_paperColor = QColor(170, 185, 220);
    m_tableColorStart = QColor(229, 219, 204);
    m_tableColorStop = QColor(252, 242, 227);
    m_fontColor = QColor(0, 0, 0);
    m_columnFontColor = QColor(Qt::darkRed);
    m_mainFont = QFont("Arial");
}

SettingsManager::~SettingsManager()
{
}

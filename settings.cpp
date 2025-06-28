#include "settings.h"
#include <QApplication>
#include <QColor>

const QString SettingsName = "settings.ini";
const QString MapSettings = "map";
const QString ModelSettings = "model";

Settings *Settings::m_instance = nullptr;
Settings *Settings::instance()
{
    if (m_instance == nullptr) {
        m_instance = new Settings();
    }
    return m_instance;
}

void Settings::initSettings()
{
    initMapSetings();
    initModelSettings();
    cacheMapSettings();
    cacheModelSettings();
}

QVariant Settings::getValue(const QString &key)
{
    return m_values.value(key, QVariant());
}

QColor Settings::getColor(const QString &key)
{
    QString value = m_values.value(key, QVariant()).toString();
    QColor color = transColor(value);
    return color;
}

Settings::Settings(QObject *parent)
    : QObject{parent}, m_settings(nullptr)
{
    QString filePath = QApplication::applicationDirPath() + "/" + SettingsName;
    m_settings = new QSettings(filePath, QSettings::IniFormat, this);
}

void Settings::initMapSetings()
{
    m_settings->beginGroup(MapSettings);
    if(!m_settings->contains(SettingsKey::MinScale))
        m_settings->setValue(SettingsKey::MinScale, 0.001);
    if(!m_settings->contains(SettingsKey::MaxScale))
        m_settings->setValue(SettingsKey::MaxScale, 0.02);
    if(!m_settings->contains(SettingsKey::ViewWidth))
        m_settings->setValue(SettingsKey::ViewWidth, 800.0);
    if(!m_settings->contains(SettingsKey::ViewHeight))
        m_settings->setValue(SettingsKey::ViewHeight, 600.0);
    if(!m_settings->contains(SettingsKey::ViewMargin))
        m_settings->setValue(SettingsKey::ViewMargin, 0.001);
    if(!m_settings->contains(SettingsKey::ControlsBackGroundColor))
        m_settings->setValue(SettingsKey::ControlsBackGroundColor, QString("200,200,200,200"));
    if(!m_settings->contains(SettingsKey::ControlsTextColor))
        m_settings->setValue(SettingsKey::ControlsTextColor, QString("0,0,0,255"));
    if(!m_settings->contains(SettingsKey::MaxZoomLevel))
        m_settings->setValue(SettingsKey::MaxZoomLevel, 12);
    if(!m_settings->contains(SettingsKey::MinZoomLevel))
        m_settings->setValue(SettingsKey::MinZoomLevel, 1);
    if(!m_settings->contains(SettingsKey::OfflineTilePath))
        m_settings->setValue(SettingsKey::OfflineTilePath, qApp->applicationDirPath() + "/maps");
    if(!m_settings->contains(SettingsKey::InitLatitude))
        m_settings->setValue(SettingsKey::InitLatitude, 39.9042);//初始化纬度，北京
    if(!m_settings->contains(SettingsKey::InitLongitude))
        m_settings->setValue(SettingsKey::InitLongitude, 116.4074);//初始化经度，北京
    if(!m_settings->contains(SettingsKey::InitScale))
        m_settings->setValue(SettingsKey::InitScale, 0.01);
    if(!m_settings->contains(SettingsKey::ItemResourcesPath))
        m_settings->setValue(SettingsKey::ItemResourcesPath, qApp->applicationDirPath() + "/item_resources");
    if(!m_settings->contains(SettingsKey::AirplaneSize))
        m_settings->setValue(SettingsKey::AirplaneSize, QSize(50, 50));
    if(!m_settings->contains(SettingsKey::AirTrackSize))
        m_settings->setValue(SettingsKey::AirTrackSize, 5000);
    if(!m_settings->contains(SettingsKey::AirTrackColor))
        m_settings->setValue(SettingsKey::AirTrackColor, QString("70,177,225,255"));
    if(!m_settings->contains(SettingsKey::AirTrackWidth))
        m_settings->setValue(SettingsKey::AirTrackWidth, 5);
    if(!m_settings->contains(SettingsKey::AreaFontSize))
        m_settings->setValue(SettingsKey::AreaFontSize, 10.0);
    if(!m_settings->contains(SettingsKey::AreaPenWidth))
        m_settings->setValue(SettingsKey::AreaPenWidth, 2.0);
    if(!m_settings->contains(SettingsKey::AreaSelectPointSize))
        m_settings->setValue(SettingsKey::AreaSelectPointSize, 8.0);
    if(!m_settings->contains(SettingsKey::AreaSelectMaxPoints))
        m_settings->setValue(SettingsKey::AreaSelectMaxPoints, 6);
    if(!m_settings->contains(SettingsKey::AreaSelectPointColor))
        m_settings->setValue(SettingsKey::AreaSelectPointColor, QString("255,255,0,180"));
    if(!m_settings->contains(SettingsKey::AreaSelectPreviewLineWidth))
        m_settings->setValue(SettingsKey::AreaSelectPreviewLineWidth, 4.0);
    if(!m_settings->contains(SettingsKey::AreaSelectPreviewLineColor))
        m_settings->setValue(SettingsKey::AreaSelectPreviewLineColor, QString("255,0,255,200"));
    if(!m_settings->contains(SettingsKey::AreaSelectLineWidth))
        m_settings->setValue(SettingsKey::AreaSelectLineWidth, 5.0);
    if(!m_settings->contains(SettingsKey::AreaSelectLineColor))
        m_settings->setValue(SettingsKey::AreaSelectLineColor, QString("0,0,255,255"));
    if(!m_settings->contains(SettingsKey::RocketSize))
        m_settings->setValue(SettingsKey::RocketSize, QSize(50, 50));
    m_settings->endGroup();
}

void Settings::initModelSettings()
{
    m_settings->beginGroup(ModelSettings);
    if(!m_settings->contains(SettingsKey::TaskAreaColor))
        m_settings->setValue(SettingsKey::TaskAreaColor, QString("51,136,255,80"));
    if(!m_settings->contains(SettingsKey::NoFlayAreaColor))
        m_settings->setValue(SettingsKey::NoFlayAreaColor, QString("51,204,153,80"));
    if(!m_settings->contains(SettingsKey::ThreatAreaColor))
        m_settings->setValue(SettingsKey::ThreatAreaColor, QString("255,204,51,80"));
    if(!m_settings->contains(SettingsKey::AreaMax))
        m_settings->setValue(SettingsKey::AreaMax, 255);
    m_settings->endGroup();
}

void Settings::cacheMapSettings()
{
    m_settings->beginGroup(MapSettings);
    if(m_settings->contains(SettingsKey::MinScale))
        m_values.insert(SettingsKey::MinScale, m_settings->value(SettingsKey::MinScale));
    if(m_settings->contains(SettingsKey::MaxScale))
        m_values.insert(SettingsKey::MaxScale, m_settings->value(SettingsKey::MaxScale));
    if(m_settings->contains(SettingsKey::ViewWidth))
        m_values.insert(SettingsKey::ViewWidth, m_settings->value(SettingsKey::ViewWidth));
    if(m_settings->contains(SettingsKey::ViewHeight))
        m_values.insert(SettingsKey::ViewHeight, m_settings->value(SettingsKey::ViewHeight));
    if(m_settings->contains(SettingsKey::ViewMargin))
        m_values.insert(SettingsKey::ViewMargin, m_settings->value(SettingsKey::ViewMargin));
    if(m_settings->contains(SettingsKey::ControlsBackGroundColor))
        m_values.insert(SettingsKey::ControlsBackGroundColor, m_settings->value(SettingsKey::ControlsBackGroundColor));
    if(m_settings->contains(SettingsKey::ControlsTextColor))
        m_values.insert(SettingsKey::ControlsTextColor, m_settings->value(SettingsKey::ControlsTextColor));
    if(m_settings->contains(SettingsKey::MaxZoomLevel))
        m_values.insert(SettingsKey::MaxZoomLevel, m_settings->value(SettingsKey::MaxZoomLevel));
    if(m_settings->contains(SettingsKey::MinZoomLevel))
        m_values.insert(SettingsKey::MinZoomLevel, m_settings->value(SettingsKey::MinZoomLevel));
    if(m_settings->contains(SettingsKey::OfflineTilePath))
        m_values.insert(SettingsKey::OfflineTilePath, m_settings->value(SettingsKey::OfflineTilePath));
    if(m_settings->contains(SettingsKey::InitLatitude))
        m_values.insert(SettingsKey::InitLatitude, m_settings->value(SettingsKey::InitLatitude));
    if(m_settings->contains(SettingsKey::InitLongitude))
        m_values.insert(SettingsKey::InitLongitude, m_settings->value(SettingsKey::InitLongitude));
    if(m_settings->contains(SettingsKey::InitScale))
        m_values.insert(SettingsKey::InitScale, m_settings->value(SettingsKey::InitScale));
    if(m_settings->contains(SettingsKey::ItemResourcesPath))
        m_values.insert(SettingsKey::ItemResourcesPath, m_settings->value(SettingsKey::ItemResourcesPath));
    if(m_settings->contains(SettingsKey::AirplaneSize))
        m_values.insert(SettingsKey::AirplaneSize, m_settings->value(SettingsKey::AirplaneSize));
    if(m_settings->contains(SettingsKey::TankSize))
        m_values.insert(SettingsKey::TankSize, m_settings->value(SettingsKey::TankSize));
    if(m_settings->contains(SettingsKey::AirTrackSize))
        m_values.insert(SettingsKey::AirTrackSize, m_settings->value(SettingsKey::AirTrackSize));
    if(m_settings->contains(SettingsKey::AirTrackColor))
        m_values.insert(SettingsKey::AirTrackColor, m_settings->value(SettingsKey::AirTrackColor));
    if(m_settings->contains(SettingsKey::AirTrackWidth))
        m_values.insert(SettingsKey::AirTrackWidth, m_settings->value(SettingsKey::AirTrackWidth));
    if(m_settings->contains(SettingsKey::AreaFontSize))
        m_values.insert(SettingsKey::AreaFontSize, m_settings->value(SettingsKey::AreaFontSize));
    if(m_settings->contains(SettingsKey::AreaPenWidth))
        m_values.insert(SettingsKey::AreaPenWidth, m_settings->value(SettingsKey::AreaPenWidth));
    if(m_settings->contains(SettingsKey::AreaSelectPointSize))
        m_values.insert(SettingsKey::AreaSelectPointSize, m_settings->value(SettingsKey::AreaSelectPointSize));
    if(m_settings->contains(SettingsKey::AreaSelectMaxPoints))
        m_values.insert(SettingsKey::AreaSelectMaxPoints, m_settings->value(SettingsKey::AreaSelectMaxPoints));
    if(m_settings->contains(SettingsKey::AreaSelectPointColor))
        m_values.insert(SettingsKey::AreaSelectPointColor, m_settings->value(SettingsKey::AreaSelectPointColor));
    if(m_settings->contains(SettingsKey::AreaSelectPreviewLineWidth))
        m_values.insert(SettingsKey::AreaSelectPreviewLineWidth, m_settings->value(SettingsKey::AreaSelectPreviewLineWidth));
    if(m_settings->contains(SettingsKey::AreaSelectPreviewLineColor))
        m_values.insert(SettingsKey::AreaSelectPreviewLineColor, m_settings->value(SettingsKey::AreaSelectPreviewLineColor));
    if(m_settings->contains(SettingsKey::AreaSelectLineWidth))
        m_values.insert(SettingsKey::AreaSelectLineWidth, m_settings->value(SettingsKey::AreaSelectLineWidth));
    if(m_settings->contains(SettingsKey::AreaSelectLineColor))
        m_values.insert(SettingsKey::AreaSelectLineColor, m_settings->value(SettingsKey::AreaSelectLineColor));
    if(m_settings->contains(SettingsKey::RocketSize))
        m_values.insert(SettingsKey::RocketSize, m_settings->value(SettingsKey::RocketSize));
    m_settings->endGroup();
}

void Settings::cacheModelSettings()
{
    m_settings->beginGroup(ModelSettings);
    if(m_settings->contains(SettingsKey::TaskAreaColor))
        m_values.insert(SettingsKey::TaskAreaColor, m_settings->value(SettingsKey::TaskAreaColor));
    if(m_settings->contains(SettingsKey::NoFlayAreaColor))
        m_values.insert(SettingsKey::NoFlayAreaColor, m_settings->value(SettingsKey::NoFlayAreaColor));
    if(m_settings->contains(SettingsKey::ThreatAreaColor))
        m_values.insert(SettingsKey::ThreatAreaColor, m_settings->value(SettingsKey::ThreatAreaColor));
    if(m_settings->contains(SettingsKey::AreaMax))
        m_values.insert(SettingsKey::AreaMax, m_settings->value(SettingsKey::AreaMax));
    m_settings->endGroup();
}

QColor Settings::transColor(const QString &value)
{
    QStringList values = value.split(",");
    if(values.size() == 4) {
        int r = values[0].toInt();
        int g = values[1].toInt();
        int b = values[2].toInt();
        int a = values[3].toInt();
        return QColor(r,g,b,a);
    }
    return QColor();
}

#include "simulationsettings.h"
#include <QApplication>

const QString SimulationSettingsName = "simulation_settings.ini";
const QString AirplaneSettings = "airplane";

SimulationSettings *SimulationSettings::m_instance = nullptr;

SimulationSettings *SimulationSettings::instance()
{
    if (m_instance == nullptr) {
        m_instance = new SimulationSettings();
    }
    return m_instance;
}

void SimulationSettings::initSettings()
{
    initAirplaneSettings();
    cacheAirplaneSettings();
}

QVariant SimulationSettings::getValue(const QString &key)
{
    return m_values.value(key, QVariant());
}

double SimulationSettings::getDouble(const QString &key)
{
    return m_values.value(key, QVariant()).toDouble();
}

void SimulationSettings::setValue(const QString &key, const QVariant &value)
{
    m_values.insert(key, value);
}

void SimulationSettings::setDouble(const QString &key, double value)
{
    setValue(key, QVariant(value));
}

SimulationSettings::SimulationSettings(QObject *parent)
    : QObject{parent}, m_settings(nullptr)
{
    QString filePath = QApplication::applicationDirPath() + "/" + SimulationSettingsName;
    m_settings = new QSettings(filePath, QSettings::IniFormat, this);
}

void SimulationSettings::initAirplaneSettings()
{
    m_settings->beginGroup(AirplaneSettings);
    if(!m_settings->contains(SimulationSettingsKey::TakeoffSpeed))
        m_settings->setValue(SimulationSettingsKey::TakeoffSpeed, 300.0);
    if(!m_settings->contains(SimulationSettingsKey::CruiseSpeed))
        m_settings->setValue(SimulationSettingsKey::CruiseSpeed, 500.0);
    if(!m_settings->contains(SimulationSettingsKey::LandingSpeed))
        m_settings->setValue(SimulationSettingsKey::LandingSpeed, 200.0);
    if(!m_settings->contains(SimulationSettingsKey::TurnRate))
        m_settings->setValue(SimulationSettingsKey::TurnRate, 30.0);
    if(!m_settings->contains(SimulationSettingsKey::ClimbRate))
        m_settings->setValue(SimulationSettingsKey::ClimbRate, 50.0);
    if(!m_settings->contains(SimulationSettingsKey::DescentRate))
        m_settings->setValue(SimulationSettingsKey::DescentRate, 30.0);
    if(!m_settings->contains(SimulationSettingsKey::WaypointRadius))
        m_settings->setValue(SimulationSettingsKey::WaypointRadius, 500.0);
    if(!m_settings->contains(SimulationSettingsKey::SimulationInterval))
        m_settings->setValue(SimulationSettingsKey::SimulationInterval, 100);
    m_settings->endGroup();
}

void SimulationSettings::cacheAirplaneSettings()
{
    m_settings->beginGroup(AirplaneSettings);
    if(m_settings->contains(SimulationSettingsKey::TakeoffSpeed))
        m_values.insert(SimulationSettingsKey::TakeoffSpeed, m_settings->value(SimulationSettingsKey::TakeoffSpeed));
    if(m_settings->contains(SimulationSettingsKey::CruiseSpeed))
        m_values.insert(SimulationSettingsKey::CruiseSpeed, m_settings->value(SimulationSettingsKey::CruiseSpeed));
    if(m_settings->contains(SimulationSettingsKey::LandingSpeed))
        m_values.insert(SimulationSettingsKey::LandingSpeed, m_settings->value(SimulationSettingsKey::LandingSpeed));
    if(m_settings->contains(SimulationSettingsKey::TurnRate))
        m_values.insert(SimulationSettingsKey::TurnRate, m_settings->value(SimulationSettingsKey::TurnRate));
    if(m_settings->contains(SimulationSettingsKey::ClimbRate))
        m_values.insert(SimulationSettingsKey::ClimbRate, m_settings->value(SimulationSettingsKey::ClimbRate));
    if(m_settings->contains(SimulationSettingsKey::DescentRate))
        m_values.insert(SimulationSettingsKey::DescentRate, m_settings->value(SimulationSettingsKey::DescentRate));
    if(m_settings->contains(SimulationSettingsKey::WaypointRadius))
        m_values.insert(SimulationSettingsKey::WaypointRadius, m_settings->value(SimulationSettingsKey::WaypointRadius));
    if(m_settings->contains(SimulationSettingsKey::SimulationInterval))
        m_values.insert(SimulationSettingsKey::SimulationInterval, m_settings->value(SimulationSettingsKey::SimulationInterval));
    m_settings->endGroup();
}

void SimulationSettings::saveSettings()
{
    m_settings->beginGroup(AirplaneSettings);
    m_settings->setValue(SimulationSettingsKey::SimulationInterval, m_values.value(SimulationSettingsKey::SimulationInterval).toInt());
    m_settings->setValue(SimulationSettingsKey::TakeoffSpeed, m_values.value(SimulationSettingsKey::TakeoffSpeed).toDouble());
    m_settings->setValue(SimulationSettingsKey::CruiseSpeed, m_values.value(SimulationSettingsKey::CruiseSpeed).toDouble());
    m_settings->setValue(SimulationSettingsKey::LandingSpeed, m_values.value(SimulationSettingsKey::LandingSpeed).toDouble());
    m_settings->setValue(SimulationSettingsKey::TurnRate, m_values.value(SimulationSettingsKey::TurnRate).toDouble());
    m_settings->setValue(SimulationSettingsKey::ClimbRate, m_values.value(SimulationSettingsKey::ClimbRate).toDouble());
    m_settings->setValue(SimulationSettingsKey::DescentRate, m_values.value(SimulationSettingsKey::DescentRate).toDouble());
    m_settings->setValue(SimulationSettingsKey::WaypointRadius, m_values.value(SimulationSettingsKey::WaypointRadius).toDouble());
    m_settings->setValue(SimulationSettingsKey::SimulationInterval, m_values.value(SimulationSettingsKey::SimulationInterval).toInt());
    m_settings->endGroup();
    m_settings->sync();
}

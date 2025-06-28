#ifndef SIMULATIONSETTINGS_H
#define SIMULATIONSETTINGS_H

#include <QObject>
#include <QScopedPointer>
#include <QSettings>
#include <QHash>

namespace SimulationSettingsKey {
    // Airplane simulation constants
    const QString TakeoffSpeed = "takeoff_speed";
    const QString CruiseSpeed = "cruise_speed";
    const QString LandingSpeed = "landing_speed";
    const QString TurnRate = "turn_rate";
    const QString ClimbRate = "climb_rate";
    const QString DescentRate = "descent_rate";
    const QString WaypointRadius = "waypoint_radius";
    const QString SimulationInterval = "simulation_interval";
}

class SimulationSettings : public QObject
{
    Q_OBJECT
public:
    static SimulationSettings* instance();

    void initSettings();
    QVariant getValue(const QString &key);
    double getDouble(const QString &key);
    void setValue(const QString &key, const QVariant &value);
    void setDouble(const QString &key, double value);
    void saveSettings();

private:
    explicit SimulationSettings(QObject *parent = nullptr);
    void initAirplaneSettings();
    void cacheAirplaneSettings();

private:
    QSettings *m_settings;
    QHash<QString, QVariant> m_values;
    static SimulationSettings *m_instance;
};

#endif // SIMULATIONSETTINGS_H

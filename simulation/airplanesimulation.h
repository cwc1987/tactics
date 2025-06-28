#ifndef AIRPLANESIMULATION_H
#define AIRPLANESIMULATION_H

#include <QThread>
#include <QList>
#include <QTimer>
#include <QMutex>
#include <QGVGlobal.h>

class AirPlaneItem;
class AirplaneSimulation : public QThread
{
    Q_OBJECT
public:
    explicit AirplaneSimulation(QObject *parent = nullptr);
    void initAirplane();
    void startSimulation();
    void stopSimulation();
    void reloadSettings();

signals:
    void airplaneInited(const QList<AirPlaneItem> &items);
    void airplaneUpdated(const AirPlaneItem &airplane);

protected:
    void run() override;

private:
    void updateFlightData();
    void calculateFlightPath(int airplaneIndex);
    void updateAirplanePosition(int airplaneIndex);
    void updateAirplaneHeading(int airplaneIndex);
    void updateAirplaneSpeed(int airplaneIndex);
    void updateFlightStatus(int airplaneIndex);
    double calculateDistance(const QGV::GeoPos &pos1, const QGV::GeoPos &pos2);
    double calculateBearing(const QGV::GeoPos &from, const QGV::GeoPos &to);
    QGV::GeoPos interpolatePosition(const QGV::GeoPos &start, const QGV::GeoPos &end, double progress);

private:
    QList<AirPlaneItem> m_airplaneitem;
    QVector<QList<QGV::GeoPos>> m_waypoints;  // Waypoints for each airplane
    QVector<int> m_currentWaypointIndex;      // Current waypoint index for each airplane
    QVector<double> m_waypointProgress;       // Progress between waypoints (0.0 to 1.0)
    QVector<bool> m_isFlying;                 // Flight status for each airplane
    QVector<QDateTime> m_takeoffTime;         // Takeoff time for each airplane
    
    bool m_simulationRunning;
    int m_simulationInterval;

    double m_takeoffSpeed;
    double m_cruiseSpeed;
    double m_landingSpeed;
    double m_turnRate;
    double m_climbRate;
    double m_descentRate;
    double m_waypointRadius;
};

#endif // AIRPLANESIMULATION_H

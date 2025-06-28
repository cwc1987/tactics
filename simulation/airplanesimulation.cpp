#include "airplanesimulation.h"
#include "airplanedatas.h"
#include "airplanemodel.h"
#include "simulationsettings.h"
#include <QDateTime>
#include <QTimer>
#include <QMutex>
#include <QDebug>
#include <QtMath>

AirplaneSimulation::AirplaneSimulation(QObject *parent)
    : QThread{parent}
    , m_simulationRunning(false)
    , m_simulationInterval(100)  // Default value, will be updated after settings init
    , m_takeoffSpeed(300.0)      // Default value, will be updated after settings init
    , m_cruiseSpeed(500.0)       // Default value, will be updated after settings init
    , m_landingSpeed(200.0)      // Default value, will be updated after settings init
    , m_turnRate(30.0)           // Default value, will be updated after settings init
    , m_climbRate(50.0)          // Default value, will be updated after settings init
    , m_descentRate(30.0)        // Default value, will be updated after settings init
    , m_waypointRadius(500.0)    // Default value, will be updated after settings init
{    
    
    // Update parameters from settings
    m_simulationInterval = SimulationSettings::instance()->getValue(SimulationSettingsKey::SimulationInterval).toInt();
    m_takeoffSpeed = SimulationSettings::instance()->getDouble(SimulationSettingsKey::TakeoffSpeed);
    m_cruiseSpeed = SimulationSettings::instance()->getDouble(SimulationSettingsKey::CruiseSpeed);
    m_landingSpeed = SimulationSettings::instance()->getDouble(SimulationSettingsKey::LandingSpeed);
    m_turnRate = SimulationSettings::instance()->getDouble(SimulationSettingsKey::TurnRate);
    m_climbRate = SimulationSettings::instance()->getDouble(SimulationSettingsKey::ClimbRate);
    m_descentRate = SimulationSettings::instance()->getDouble(SimulationSettingsKey::DescentRate);
    m_waypointRadius = SimulationSettings::instance()->getDouble(SimulationSettingsKey::WaypointRadius);
    
    // Initialize flight data lists
    m_waypoints.resize(4);
    m_currentWaypointIndex.resize(4);
    m_waypointProgress.resize(4);
    m_takeoffTime.resize(4);
    
    for (int i = 0; i < 4; ++i) {
        m_currentWaypointIndex[i] = 0;
        m_waypointProgress[i] = 0.0;
        m_takeoffTime[i] = QDateTime::currentDateTime();
    }
}

void AirplaneSimulation::initAirplane()
{    
    // Clear existing airplane items
    m_airplaneitem.clear();
    
    // Create airplanedatas instance to read configuration
    AirplaneDatas airplaneDatas;
    
    // Initialize 4 airplanes from settings
    for (int i = 1; i <= 4; ++i) {
        AirPlaneItem airplane;
        
        // Set basic information
        airplane.id = airplaneDatas.getValue(QString("airplane%1_id").arg(i)).toString();
        airplane.name = airplaneDatas.getValue(QString("airplane%1_name").arg(i)).toString();
        airplane.type = airplaneDatas.getValue(QString("airplane%1_type").arg(i)).toString();
        airplane.model = airplaneDatas.getValue(QString("airplane%1_model").arg(i)).toString();
        
        // Set position information
        airplane.latitude = airplaneDatas.getValue(QString("airplane%1_latitude").arg(i)).toDouble();
        airplane.longitude = airplaneDatas.getValue(QString("airplane%1_longitude").arg(i)).toDouble();
        airplane.altitude = airplaneDatas.getValue(QString("airplane%1_altitude").arg(i)).toDouble();
        airplane.heading = airplaneDatas.getValue(QString("airplane%1_heading").arg(i)).toDouble();
        airplane.speed = airplaneDatas.getValue(QString("airplane%1_speed").arg(i)).toDouble();
        airplane.verticalSpeed = airplaneDatas.getValue(QString("airplane%1_vertical_speed").arg(i)).toDouble();
        
        // Set state information
        int stateValue = airplaneDatas.getValue(QString("airplane%1_state").arg(i)).toInt();
        switch (stateValue) {
            case 0:
                airplane.state = AirPlaneItem::Normal;
                break;
            case 1:
                airplane.state = AirPlaneItem::Damage;
                break;
            case 2:
                airplane.state = AirPlaneItem::Emergency;
                break;
            case 3:
                airplane.state = AirPlaneItem::Maintenance;
                break;
            case 4:
                airplane.state = AirPlaneItem::Offline;
                break;
            default:
                airplane.state = AirPlaneItem::Normal;
                break;
        }
        
        // Set flight information
        airplane.flightDuration = airplaneDatas.getValue(QString("airplane%1_flight_duration").arg(i)).toDouble();
        airplane.totalDistance = airplaneDatas.getValue(QString("airplane%1_total_distance").arg(i)).toDouble();
        airplane.remainingOil = airplaneDatas.getValue(QString("airplane%1_remaining_oil").arg(i)).toDouble();
        airplane.signalStrength = airplaneDatas.getValue(QString("airplane%1_signal_strength").arg(i)).toDouble();
        
        // Set sensor information
        airplane.cameraOnline = airplaneDatas.getValue(QString("airplane%1_camera_online").arg(i)).toBool();
        airplane.gpsOnline = airplaneDatas.getValue(QString("airplane%1_gps_online").arg(i)).toBool();
        airplane.radarOnline = airplaneDatas.getValue(QString("airplane%1_radar_online").arg(i)).toBool();
        
        // Set control information
        int controlStateValue = airplaneDatas.getValue(QString("airplane%1_control_state").arg(i)).toInt();
        switch (controlStateValue) {
            case 0:
                airplane.controlState = AirPlaneItem::Manual;
                break;
            case 1:
                airplane.controlState = AirPlaneItem::Auto;
                break;
            case 3:
                airplane.controlState = AirPlaneItem::Unknown;
                break;
            default:
                airplane.controlState = AirPlaneItem::Manual;
                break;
        }
        
        // Set communication information
        airplane.communicationChannel = airplaneDatas.getValue(QString("airplane%1_communication_channel").arg(i)).toString();
        airplane.communicationQuality = airplaneDatas.getValue(QString("airplane%1_communication_quality").arg(i)).toDouble();
        airplane.lastCommunication = QDateTime::currentDateTime();
        
        // Set default takeoff and landing times
        airplane.takeoffTime = QDateTime::currentDateTime();
        airplane.landingTime = QDateTime::currentDateTime();
        airplane.isActive = false;
        
        // Add airplane to the list
        m_airplaneitem.append(airplane);
        
        // Load waypoints for this airplane
        m_waypoints[i-1] = airplaneDatas.getWayPoints(i);
        
        // Initialize flight path
        calculateFlightPath(i-1);
    }

    emit airplaneInited(m_airplaneitem);
}

void AirplaneSimulation::startSimulation()
{
    if (!m_simulationRunning) {
        m_simulationRunning = true;
        this->start();
        qDebug() << "Flight simulation started";
    }
}

void AirplaneSimulation::stopSimulation()
{
    if (m_simulationRunning) {
        m_simulationRunning = false;
        qDebug() << "Flight simulation stopped";
    }
}

void AirplaneSimulation::reloadSettings()
{
    if(m_simulationRunning)
    {
        return;
    }
    
    // Update parameters from settings
    m_simulationInterval = SimulationSettings::instance()->getValue(SimulationSettingsKey::SimulationInterval).toInt();
    m_takeoffSpeed = SimulationSettings::instance()->getDouble(SimulationSettingsKey::TakeoffSpeed);
    m_cruiseSpeed = SimulationSettings::instance()->getDouble(SimulationSettingsKey::CruiseSpeed);
    m_landingSpeed = SimulationSettings::instance()->getDouble(SimulationSettingsKey::LandingSpeed);
    m_turnRate = SimulationSettings::instance()->getDouble(SimulationSettingsKey::TurnRate);
    m_climbRate = SimulationSettings::instance()->getDouble(SimulationSettingsKey::ClimbRate);
    m_descentRate = SimulationSettings::instance()->getDouble(SimulationSettingsKey::DescentRate);
    m_waypointRadius = SimulationSettings::instance()->getDouble(SimulationSettingsKey::WaypointRadius);
    
    qDebug() << "Airplane simulation settings reloaded";
}

void AirplaneSimulation::run()
{
    while(m_simulationRunning) {
        updateFlightData();
        msleep(m_simulationInterval);
    }
}

void AirplaneSimulation::updateFlightData()
{    
    for (int i = 0; i < m_airplaneitem.size(); ++i) {
        if (m_airplaneitem[i].state == AirPlaneItem::Normal) {
            updateAirplanePosition(i);
            updateAirplaneHeading(i);
            updateAirplaneSpeed(i);
            updateFlightStatus(i);
            
            // Emit update signal
            emit airplaneUpdated(m_airplaneitem[i]);
        }
    }
}

void AirplaneSimulation::calculateFlightPath(int airplaneIndex)
{
    if (airplaneIndex < 0 || airplaneIndex >= m_waypoints.size()) {
        return;
    }
    
    const QList<QGV::GeoPos> &waypoints = m_waypoints[airplaneIndex];
    if (waypoints.isEmpty()) {
        return;
    }
    
    // Set initial position to first waypoint (takeoff point)
    if (airplaneIndex < m_airplaneitem.size()) {
        m_airplaneitem[airplaneIndex].latitude = waypoints.first().latitude();
        m_airplaneitem[airplaneIndex].longitude = waypoints.first().longitude();
        m_airplaneitem[airplaneIndex].altitude = 0.0; // Start on ground
        m_airplaneitem[airplaneIndex].speed = 0.0;
        m_airplaneitem[airplaneIndex].verticalSpeed = 0.0;
    }
    
    m_currentWaypointIndex[airplaneIndex] = 0;
    m_waypointProgress[airplaneIndex] = 0.0;
    m_takeoffTime[airplaneIndex] = QDateTime::currentDateTime();
}

void AirplaneSimulation::updateAirplanePosition(int airplaneIndex)
{
    if (airplaneIndex < 0 || airplaneIndex >= m_waypoints.size() || 
        airplaneIndex >= m_airplaneitem.size()) {
        return;
    }
    
    const QList<QGV::GeoPos> &waypoints = m_waypoints[airplaneIndex];
    if (waypoints.isEmpty()) {
        return;
    }
    
    AirPlaneItem &airplane = m_airplaneitem[airplaneIndex];
    int currentIndex = m_currentWaypointIndex[airplaneIndex];
    
    // Check if we have waypoints to follow
    if (currentIndex >= waypoints.size()) {
        return; // No more waypoints
    }
    
    // If we're at the last waypoint, we're done
    if (currentIndex == waypoints.size() - 1) {
        // Set position to last waypoint
        airplane.latitude = waypoints[currentIndex].latitude();
        airplane.longitude = waypoints[currentIndex].longitude();
        return;
    }
    
    // Update position based on current waypoint
    if (currentIndex < waypoints.size() - 1) {
        QGV::GeoPos startPos = waypoints[currentIndex];
        QGV::GeoPos endPos = waypoints[currentIndex + 1];
        
        // Calculate progress between waypoints
        double progress = m_waypointProgress[airplaneIndex];
        QGV::GeoPos interpolatedPos = interpolatePosition(startPos, endPos, progress);
        
        // Update airplane position
        airplane.latitude = interpolatedPos.latitude();
        airplane.longitude = interpolatedPos.longitude();
        
        // Update progress based on speed and time
        double timeStep = m_simulationInterval / 1000.0; // Convert to seconds
        double speed = m_cruiseSpeed; // Use constant cruise speed
        double distance = calculateDistance(startPos, endPos);
        
        if (distance > 0) {
            double progressStep = (speed * timeStep) / distance;
            m_waypointProgress[airplaneIndex] += progressStep;
            
            if (m_waypointProgress[airplaneIndex] >= 1.0) {
                // Reached the next waypoint
                m_waypointProgress[airplaneIndex] = 0.0;
                m_currentWaypointIndex[airplaneIndex]++;
            }
        }
    }
}

void AirplaneSimulation::updateAirplaneHeading(int airplaneIndex)
{
    if (airplaneIndex < 0 || airplaneIndex >= m_waypoints.size() || 
        airplaneIndex >= m_airplaneitem.size()) {
        return;
    }
    
    const QList<QGV::GeoPos> &waypoints = m_waypoints[airplaneIndex];
    if (waypoints.isEmpty()) {
        return;
    }
    
    AirPlaneItem &airplane = m_airplaneitem[airplaneIndex];
    int currentIndex = m_currentWaypointIndex[airplaneIndex];
    
    if (currentIndex < waypoints.size() - 1) {
        QGV::GeoPos currentPos(airplane.latitude, airplane.longitude);
        QGV::GeoPos targetPos = waypoints[currentIndex + 1];
        
        double targetBearing = calculateBearing(currentPos, targetPos);
        double currentBearing = airplane.heading;
        
        // Calculate heading difference
        double bearingDiff = targetBearing - currentBearing;
        while (bearingDiff > 180.0) bearingDiff -= 360.0;
        while (bearingDiff < -180.0) bearingDiff += 360.0;
        
        // Gradually turn towards target bearing
        double timeStep = m_simulationInterval / 1000.0;
        double turnAmount = m_turnRate * timeStep;
        
        if (qAbs(bearingDiff) > turnAmount) {
            if (bearingDiff > 0) {
                airplane.heading += turnAmount;
            } else {
                airplane.heading -= turnAmount;
            }
        } else {
            airplane.heading = targetBearing;
        }
        
        // Normalize heading to 0-360 range
        while (airplane.heading >= 360.0) airplane.heading -= 360.0;
        while (airplane.heading < 0.0) airplane.heading += 360.0;
    }
}

void AirplaneSimulation::updateAirplaneSpeed(int airplaneIndex)
{
    if (airplaneIndex < 0 || airplaneIndex >= m_airplaneitem.size()) {
        return;
    }
    
    AirPlaneItem &airplane = m_airplaneitem[airplaneIndex];
    
    // Use constant cruise speed for all phases
    airplane.speed = m_cruiseSpeed;
    airplane.verticalSpeed = 0.0; // No vertical movement for simplicity
    
    // Keep altitude constant
    if (airplane.altitude < 1000.0) {
        airplane.altitude = 1000.0; // Set to cruise altitude
    }
}

void AirplaneSimulation::updateFlightStatus(int airplaneIndex)
{
    if (airplaneIndex < 0 || airplaneIndex >= m_airplaneitem.size()) {
        return;
    }
    
    AirPlaneItem &airplane = m_airplaneitem[airplaneIndex];
    
    // Update flight duration
    QDateTime now = QDateTime::currentDateTime();
    qint64 flightTimeMs = m_takeoffTime[airplaneIndex].msecsTo(now);
    airplane.flightDuration = flightTimeMs / 60000.0; // Convert to minutes
    
    // Update total distance (simplified calculation)
    double timeStep = m_simulationInterval / 1000.0;
    airplane.totalDistance += airplane.speed * timeStep;
    
    // Update remaining oil (gradually decrease)
    if (airplane.remainingOil > 0.0) {
        double timeStep = m_simulationInterval / 1000.0;
        double oilConsumption = 0.1 * timeStep; // 0.1% per second
        airplane.remainingOil -= oilConsumption;
        if (airplane.remainingOil < 0.0) {
            airplane.remainingOil = 0.0;
        }
    }
    
    // Update signal strength (simulate variations)
    double variation = (qrand() % 100 - 50) / 1000.0; // ±5% variation
    airplane.signalStrength += variation;

    // Keep signal strength within reasonable bounds
    if (airplane.signalStrength > 100.0) airplane.signalStrength = 100.0;
    if (airplane.signalStrength < 50.0) airplane.signalStrength = 50.0;
    
    // Update communication time
    airplane.lastCommunication = QDateTime::currentDateTime();
}

double AirplaneSimulation::calculateDistance(const QGV::GeoPos &pos1, const QGV::GeoPos &pos2)
{
    const double R = 6371000.0; // Earth radius in meters
    
    double lat1 = qDegreesToRadians(pos1.latitude());
    double lat2 = qDegreesToRadians(pos2.latitude());
    double deltaLat = qDegreesToRadians(pos2.latitude() - pos1.latitude());
    double deltaLon = qDegreesToRadians(pos2.longitude() - pos1.longitude());
    
    double a = qSin(deltaLat / 2) * qSin(deltaLat / 2) +
               qCos(lat1) * qCos(lat2) *
               qSin(deltaLon / 2) * qSin(deltaLon / 2);
    double c = 2 * qAtan2(qSqrt(a), qSqrt(1 - a));
    
    return R * c;
}

double AirplaneSimulation::calculateBearing(const QGV::GeoPos &from, const QGV::GeoPos &to)
{
    double lat1 = qDegreesToRadians(from.latitude());
    double lat2 = qDegreesToRadians(to.latitude());
    double deltaLon = qDegreesToRadians(to.longitude() - from.longitude());
    
    double y = qSin(deltaLon) * qCos(lat2);
    double x = qCos(lat1) * qSin(lat2) - qSin(lat1) * qCos(lat2) * qCos(deltaLon);
    
    double bearing = qRadiansToDegrees(qAtan2(y, x));
    return (bearing + 360.0) - qFloor((bearing + 360.0) / 360.0) * 360.0;
}

QGV::GeoPos AirplaneSimulation::interpolatePosition(const QGV::GeoPos &start, const QGV::GeoPos &end, double progress)
{
    if (progress <= 0.0) return start;
    if (progress >= 1.0) return end;
    
    double lat = start.latitude() + (end.latitude() - start.latitude()) * progress;
    double lon = start.longitude() + (end.longitude() - start.longitude()) * progress;
    
    return QGV::GeoPos(lat, lon);
}

#include "airplanedatas.h"
#include <QApplication>

AirplaneDatas::AirplaneDatas(QObject *parent)
    : QObject{parent}
{
    QString filePath = QApplication::applicationDirPath() + "/" + "airplane_datas.ini";
    m_settings = new QSettings(filePath, QSettings::IniFormat, this);
    initSettings();
    cacheSettings();
}

AirplaneDatas::~AirplaneDatas()
{
}

QList<QGV::GeoPos> AirplaneDatas::getWayPoints(int airplaneIndex)
{
    QList<QGV::GeoPos> wayPoints;

    if (airplaneIndex < 1 || airplaneIndex > 4) {
        return wayPoints;
    }

    QString prefix = QString("airplane%1_waypoint").arg(airplaneIndex);

    for (int i = 1; i <= 10; ++i) {
        QString latKey = QString("%1%2_lat").arg(prefix).arg(i);
        QString lonKey = QString("%1%2_lon").arg(prefix).arg(i);

        if (m_values.contains(latKey) && m_values.contains(lonKey)) {
            double lat = m_values.value(latKey).toDouble();
            double lon = m_values.value(lonKey).toDouble();

            if (lat != 0.0 && lon != 0.0) {
                QGV::GeoPos wayPoint(lat, lon);
                wayPoints.append(wayPoint);
            }
        }
    }

    return wayPoints;
}

QVariant AirplaneDatas::getValue(const QString key) const
{
    return m_values.value(key, QVariant());
}

void AirplaneDatas::setValue(const QString &key, const QVariant &value)
{
    m_values.insert(key, value);
}

void AirplaneDatas::initSettings()
{
    // 无人机1配置
    if(!m_settings->contains(AirplaneDataKey::Airplane1ID))
        m_settings->setValue(AirplaneDataKey::Airplane1ID, "1");
    if(!m_settings->contains(AirplaneDataKey::Airplane1Name))
        m_settings->setValue(AirplaneDataKey::Airplane1Name, "Recon1");
    if(!m_settings->contains(AirplaneDataKey::Airplane1Type))
        m_settings->setValue(AirplaneDataKey::Airplane1Type, "Reconnaissance");
    if(!m_settings->contains(AirplaneDataKey::Airplane1Model))
        m_settings->setValue(AirplaneDataKey::Airplane1Model, "LQ-207");

    if(!m_settings->contains(AirplaneDataKey::Airplane1Latitude))
        m_settings->setValue(AirplaneDataKey::Airplane1Latitude, 39.9042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1Longitude))
        m_settings->setValue(AirplaneDataKey::Airplane1Longitude, 116.4074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1Altitude))
        m_settings->setValue(AirplaneDataKey::Airplane1Altitude, 1000.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane1Heading))
        m_settings->setValue(AirplaneDataKey::Airplane1Heading, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane1Speed))
        m_settings->setValue(AirplaneDataKey::Airplane1Speed, 50.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane1VerticalSpeed))
        m_settings->setValue(AirplaneDataKey::Airplane1VerticalSpeed, 0.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane1State))
        m_settings->setValue(AirplaneDataKey::Airplane1State, 0); // Normal
    if(!m_settings->contains(AirplaneDataKey::Airplane1FlightDuration))
        m_settings->setValue(AirplaneDataKey::Airplane1FlightDuration, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane1TotalDistance))
        m_settings->setValue(AirplaneDataKey::Airplane1TotalDistance, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane1RemainingOil))
        m_settings->setValue(AirplaneDataKey::Airplane1RemainingOil, 100.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane1SignalStrength))
        m_settings->setValue(AirplaneDataKey::Airplane1SignalStrength, 100.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane1CameraOnline))
        m_settings->setValue(AirplaneDataKey::Airplane1CameraOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane1GpsOnline))
        m_settings->setValue(AirplaneDataKey::Airplane1GpsOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane1RadarOnline))
        m_settings->setValue(AirplaneDataKey::Airplane1RadarOnline, true);

    if(!m_settings->contains(AirplaneDataKey::Airplane1ControlState))
        m_settings->setValue(AirplaneDataKey::Airplane1ControlState, 0); // Manual
    if(!m_settings->contains(AirplaneDataKey::Airplane1CommunicationChannel))
        m_settings->setValue(AirplaneDataKey::Airplane1CommunicationChannel, "CH001");
    if(!m_settings->contains(AirplaneDataKey::Airplane1CommunicationQuality))
        m_settings->setValue(AirplaneDataKey::Airplane1CommunicationQuality, 100.0);

    // 无人机2配置
    if(!m_settings->contains(AirplaneDataKey::Airplane2ID))
        m_settings->setValue(AirplaneDataKey::Airplane2ID, "2");
    if(!m_settings->contains(AirplaneDataKey::Airplane2Name))
        m_settings->setValue(AirplaneDataKey::Airplane2Name, "Recon2");
    if(!m_settings->contains(AirplaneDataKey::Airplane2Type))
        m_settings->setValue(AirplaneDataKey::Airplane2Type, "Reconnaissance");
    if(!m_settings->contains(AirplaneDataKey::Airplane2Model))
        m_settings->setValue(AirplaneDataKey::Airplane2Model, "LQ-207");

    if(!m_settings->contains(AirplaneDataKey::Airplane2Latitude))
        m_settings->setValue(AirplaneDataKey::Airplane2Latitude, 39.9142);
    if(!m_settings->contains(AirplaneDataKey::Airplane2Longitude))
        m_settings->setValue(AirplaneDataKey::Airplane2Longitude, 116.4174);
    if(!m_settings->contains(AirplaneDataKey::Airplane2Altitude))
        m_settings->setValue(AirplaneDataKey::Airplane2Altitude, 1200.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane2Heading))
        m_settings->setValue(AirplaneDataKey::Airplane2Heading, 45.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane2Speed))
        m_settings->setValue(AirplaneDataKey::Airplane2Speed, 60.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane2VerticalSpeed))
        m_settings->setValue(AirplaneDataKey::Airplane2VerticalSpeed, 0.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane2State))
        m_settings->setValue(AirplaneDataKey::Airplane2State, 0); // Normal
    if(!m_settings->contains(AirplaneDataKey::Airplane2FlightDuration))
        m_settings->setValue(AirplaneDataKey::Airplane2FlightDuration, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane2TotalDistance))
        m_settings->setValue(AirplaneDataKey::Airplane2TotalDistance, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane2RemainingOil))
        m_settings->setValue(AirplaneDataKey::Airplane2RemainingOil, 95.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane2SignalStrength))
        m_settings->setValue(AirplaneDataKey::Airplane2SignalStrength, 98.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane2CameraOnline))
        m_settings->setValue(AirplaneDataKey::Airplane2CameraOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane2GpsOnline))
        m_settings->setValue(AirplaneDataKey::Airplane2GpsOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane2RadarOnline))
        m_settings->setValue(AirplaneDataKey::Airplane2RadarOnline, true);

    if(!m_settings->contains(AirplaneDataKey::Airplane2ControlState))
        m_settings->setValue(AirplaneDataKey::Airplane2ControlState, 1); // Auto
    if(!m_settings->contains(AirplaneDataKey::Airplane2CommunicationChannel))
        m_settings->setValue(AirplaneDataKey::Airplane2CommunicationChannel, "CH002");
    if(!m_settings->contains(AirplaneDataKey::Airplane2CommunicationQuality))
        m_settings->setValue(AirplaneDataKey::Airplane2CommunicationQuality, 98.0);

    // 无人机3配置
    if(!m_settings->contains(AirplaneDataKey::Airplane3ID))
        m_settings->setValue(AirplaneDataKey::Airplane3ID, "3");
    if(!m_settings->contains(AirplaneDataKey::Airplane3Name))
        m_settings->setValue(AirplaneDataKey::Airplane3Name, "Recon3");
    if(!m_settings->contains(AirplaneDataKey::Airplane3Type))
        m_settings->setValue(AirplaneDataKey::Airplane3Type, "Reconnaissance");
    if(!m_settings->contains(AirplaneDataKey::Airplane3Model))
        m_settings->setValue(AirplaneDataKey::Airplane3Model, "LQ-208");

    if(!m_settings->contains(AirplaneDataKey::Airplane3Latitude))
        m_settings->setValue(AirplaneDataKey::Airplane3Latitude, 39.8942);
    if(!m_settings->contains(AirplaneDataKey::Airplane3Longitude))
        m_settings->setValue(AirplaneDataKey::Airplane3Longitude, 116.3974);
    if(!m_settings->contains(AirplaneDataKey::Airplane3Altitude))
        m_settings->setValue(AirplaneDataKey::Airplane3Altitude, 1500.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane3Heading))
        m_settings->setValue(AirplaneDataKey::Airplane3Heading, 90.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane3Speed))
        m_settings->setValue(AirplaneDataKey::Airplane3Speed, 40.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane3VerticalSpeed))
        m_settings->setValue(AirplaneDataKey::Airplane3VerticalSpeed, 0.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane3State))
        m_settings->setValue(AirplaneDataKey::Airplane3State, 0); // Normal
    if(!m_settings->contains(AirplaneDataKey::Airplane3FlightDuration))
        m_settings->setValue(AirplaneDataKey::Airplane3FlightDuration, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane3TotalDistance))
        m_settings->setValue(AirplaneDataKey::Airplane3TotalDistance, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane3RemainingOil))
        m_settings->setValue(AirplaneDataKey::Airplane3RemainingOil, 90.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane3SignalStrength))
        m_settings->setValue(AirplaneDataKey::Airplane3SignalStrength, 95.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane3CameraOnline))
        m_settings->setValue(AirplaneDataKey::Airplane3CameraOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane3GpsOnline))
        m_settings->setValue(AirplaneDataKey::Airplane3GpsOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane3RadarOnline))
        m_settings->setValue(AirplaneDataKey::Airplane3RadarOnline, true);

    if(!m_settings->contains(AirplaneDataKey::Airplane3ControlState))
        m_settings->setValue(AirplaneDataKey::Airplane3ControlState, 1); // Auto
    if(!m_settings->contains(AirplaneDataKey::Airplane3CommunicationChannel))
        m_settings->setValue(AirplaneDataKey::Airplane3CommunicationChannel, "CH003");
    if(!m_settings->contains(AirplaneDataKey::Airplane3CommunicationQuality))
        m_settings->setValue(AirplaneDataKey::Airplane3CommunicationQuality, 95.0);

    // 无人机4配置
    if(!m_settings->contains(AirplaneDataKey::Airplane4ID))
        m_settings->setValue(AirplaneDataKey::Airplane4ID, "4");
    if(!m_settings->contains(AirplaneDataKey::Airplane4Name))
        m_settings->setValue(AirplaneDataKey::Airplane4Name, "Recon4");
    if(!m_settings->contains(AirplaneDataKey::Airplane4Type))
        m_settings->setValue(AirplaneDataKey::Airplane4Type, "Reconnaissance");
    if(!m_settings->contains(AirplaneDataKey::Airplane4Model))
        m_settings->setValue(AirplaneDataKey::Airplane4Model, "LQ-208");

    if(!m_settings->contains(AirplaneDataKey::Airplane4Latitude))
        m_settings->setValue(AirplaneDataKey::Airplane4Latitude, 39.9242);
    if(!m_settings->contains(AirplaneDataKey::Airplane4Longitude))
        m_settings->setValue(AirplaneDataKey::Airplane4Longitude, 116.4274);
    if(!m_settings->contains(AirplaneDataKey::Airplane4Altitude))
        m_settings->setValue(AirplaneDataKey::Airplane4Altitude, 800.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane4Heading))
        m_settings->setValue(AirplaneDataKey::Airplane4Heading, 135.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane4Speed))
        m_settings->setValue(AirplaneDataKey::Airplane4Speed, 70.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane4VerticalSpeed))
        m_settings->setValue(AirplaneDataKey::Airplane4VerticalSpeed, 0.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane4State))
        m_settings->setValue(AirplaneDataKey::Airplane4State, 0); // Normal
    if(!m_settings->contains(AirplaneDataKey::Airplane4FlightDuration))
        m_settings->setValue(AirplaneDataKey::Airplane4FlightDuration, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane4TotalDistance))
        m_settings->setValue(AirplaneDataKey::Airplane4TotalDistance, 0.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane4RemainingOil))
        m_settings->setValue(AirplaneDataKey::Airplane4RemainingOil, 85.0);
    if(!m_settings->contains(AirplaneDataKey::Airplane4SignalStrength))
        m_settings->setValue(AirplaneDataKey::Airplane4SignalStrength, 92.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane4CameraOnline))
        m_settings->setValue(AirplaneDataKey::Airplane4CameraOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane4GpsOnline))
        m_settings->setValue(AirplaneDataKey::Airplane4GpsOnline, true);
    if(!m_settings->contains(AirplaneDataKey::Airplane4RadarOnline))
        m_settings->setValue(AirplaneDataKey::Airplane4RadarOnline, true);

    if(!m_settings->contains(AirplaneDataKey::Airplane4ControlState))
        m_settings->setValue(AirplaneDataKey::Airplane4ControlState, 0); // Manual
    if(!m_settings->contains(AirplaneDataKey::Airplane4CommunicationChannel))
        m_settings->setValue(AirplaneDataKey::Airplane4CommunicationChannel, "CH004");
    if(!m_settings->contains(AirplaneDataKey::Airplane4CommunicationQuality))
        m_settings->setValue(AirplaneDataKey::Airplane4CommunicationQuality, 92.0);

    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint1Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint1Lat, 39.9042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint1Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint1Lon, 116.4074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint2Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint2Lat, 39.7042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint2Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint2Lon, 116.7074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint3Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint3Lat, 39.5042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint3Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint3Lon, 116.9074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint4Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint4Lat, 39.3042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint4Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint4Lon, 117.0074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint5Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint5Lat, 39.0842);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint5Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint5Lon, 117.2009);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint6Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint6Lat, 39.3042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint6Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint6Lon, 117.0074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint7Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint7Lat, 39.5042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint7Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint7Lon, 116.9074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint8Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint8Lat, 39.7042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint8Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint8Lon, 116.7074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint9Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint9Lat, 39.9042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint9Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint9Lon, 116.4074);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint10Lat))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint10Lat, 40.9042);
    if(!m_settings->contains(AirplaneDataKey::Airplane1WayPoint10Lon))
        m_settings->setValue(AirplaneDataKey::Airplane1WayPoint10Lon, 117.4074);

    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint1Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint1Lat, 40.0799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint1Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint1Lon, 116.6031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint2Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint2Lat, 40.3799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint2Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint2Lon, 117.2031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint3Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint3Lat, 40.6799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint3Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint3Lon, 117.8031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint4Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint4Lat, 41.0799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint4Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint4Lon, 118.4031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint5Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint5Lat, 41.4799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint5Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint5Lon, 119.0031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint6Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint6Lat, 41.8057);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint6Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint6Lon, 123.4315);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint7Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint7Lat, 41.4799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint7Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint7Lon, 119.0031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint8Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint8Lat, 41.0799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint8Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint8Lon, 118.4031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint9Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint9Lat, 40.6799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint9Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint9Lon, 117.8031);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint10Lat))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint10Lat, 40.0799);
    if(!m_settings->contains(AirplaneDataKey::Airplane2WayPoint10Lon))
        m_settings->setValue(AirplaneDataKey::Airplane2WayPoint10Lon, 116.6031);

    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint1Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint1Lat, 39.5098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint1Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint1Lon, 116.4105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint2Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint2Lat, 39.3098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint2Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint2Lon, 116.0105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint3Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint3Lat, 39.1098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint3Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint3Lon, 115.6105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint4Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint4Lat, 38.9098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint4Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint4Lon, 115.2105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint5Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint5Lat, 38.7098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint5Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint5Lon, 114.8105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint6Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint6Lat, 38.5098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint6Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint6Lon, 114.4105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint7Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint7Lat, 38.0428);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint7Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint7Lon, 114.5149);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint8Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint8Lat, 38.5098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint8Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint8Lon, 114.4105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint9Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint9Lat, 39.1098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint9Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint9Lon, 115.6105);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint10Lat))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint10Lat, 39.5098);
    if(!m_settings->contains(AirplaneDataKey::Airplane3WayPoint10Lon))
        m_settings->setValue(AirplaneDataKey::Airplane3WayPoint10Lon, 116.4105);

    // 飞机4装订点 - 北京圆明园到石家庄往返飞行路径
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint1Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint1Lat, 40.0089); // 北京圆明园起点
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint1Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint1Lon, 116.2975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint2Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint2Lat, 39.8089); // 中间点1
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint2Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint2Lon, 115.8975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint3Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint3Lat, 39.6089); // 中间点2
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint3Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint3Lon, 115.4975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint4Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint4Lat, 39.4089); // 中间点3
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint4Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint4Lon, 115.0975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint5Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint5Lat, 39.2089); // 中间点4
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint5Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint5Lon, 114.6975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint6Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint6Lat, 39.0089); // 中间点5
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint6Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint6Lon, 114.2975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint7Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint7Lat, 38.8089); // 中间点6
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint7Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint7Lon, 114.0975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint8Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint8Lat, 38.0428); // 石家庄终点
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint8Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint8Lon, 114.5149);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint9Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint9Lat, 39.4089); // 返程中间点
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint9Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint9Lon, 115.0975);
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint10Lat))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint10Lat, 40.0089); // 北京圆明园终点
    if(!m_settings->contains(AirplaneDataKey::Airplane4WayPoint10Lon))
        m_settings->setValue(AirplaneDataKey::Airplane4WayPoint10Lon, 116.2975);
}

void AirplaneDatas::cacheSettings()
{

    // 无人机1配置缓存
    if(m_settings->contains(AirplaneDataKey::Airplane1ID))
        m_values.insert(AirplaneDataKey::Airplane1ID, m_settings->value(AirplaneDataKey::Airplane1ID));
    if(m_settings->contains(AirplaneDataKey::Airplane1Name))
        m_values.insert(AirplaneDataKey::Airplane1Name, m_settings->value(AirplaneDataKey::Airplane1Name));
    if(m_settings->contains(AirplaneDataKey::Airplane1Type))
        m_values.insert(AirplaneDataKey::Airplane1Type, m_settings->value(AirplaneDataKey::Airplane1Type));
    if(m_settings->contains(AirplaneDataKey::Airplane1Model))
        m_values.insert(AirplaneDataKey::Airplane1Model, m_settings->value(AirplaneDataKey::Airplane1Model));

    if(m_settings->contains(AirplaneDataKey::Airplane1Latitude))
        m_values.insert(AirplaneDataKey::Airplane1Latitude, m_settings->value(AirplaneDataKey::Airplane1Latitude));
    if(m_settings->contains(AirplaneDataKey::Airplane1Longitude))
        m_values.insert(AirplaneDataKey::Airplane1Longitude, m_settings->value(AirplaneDataKey::Airplane1Longitude));
    if(m_settings->contains(AirplaneDataKey::Airplane1Altitude))
        m_values.insert(AirplaneDataKey::Airplane1Altitude, m_settings->value(AirplaneDataKey::Airplane1Altitude));
    if(m_settings->contains(AirplaneDataKey::Airplane1Heading))
        m_values.insert(AirplaneDataKey::Airplane1Heading, m_settings->value(AirplaneDataKey::Airplane1Heading));
    if(m_settings->contains(AirplaneDataKey::Airplane1Speed))
        m_values.insert(AirplaneDataKey::Airplane1Speed, m_settings->value(AirplaneDataKey::Airplane1Speed));
    if(m_settings->contains(AirplaneDataKey::Airplane1VerticalSpeed))
        m_values.insert(AirplaneDataKey::Airplane1VerticalSpeed, m_settings->value(AirplaneDataKey::Airplane1VerticalSpeed));

    if(m_settings->contains(AirplaneDataKey::Airplane1State))
        m_values.insert(AirplaneDataKey::Airplane1State, m_settings->value(AirplaneDataKey::Airplane1State));
    if(m_settings->contains(AirplaneDataKey::Airplane1FlightDuration))
        m_values.insert(AirplaneDataKey::Airplane1FlightDuration, m_settings->value(AirplaneDataKey::Airplane1FlightDuration));
    if(m_settings->contains(AirplaneDataKey::Airplane1TotalDistance))
        m_values.insert(AirplaneDataKey::Airplane1TotalDistance, m_settings->value(AirplaneDataKey::Airplane1TotalDistance));
    if(m_settings->contains(AirplaneDataKey::Airplane1RemainingOil))
        m_values.insert(AirplaneDataKey::Airplane1RemainingOil, m_settings->value(AirplaneDataKey::Airplane1RemainingOil));
    if(m_settings->contains(AirplaneDataKey::Airplane1SignalStrength))
        m_values.insert(AirplaneDataKey::Airplane1SignalStrength, m_settings->value(AirplaneDataKey::Airplane1SignalStrength));

    if(m_settings->contains(AirplaneDataKey::Airplane1CameraOnline))
        m_values.insert(AirplaneDataKey::Airplane1CameraOnline, m_settings->value(AirplaneDataKey::Airplane1CameraOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane1GpsOnline))
        m_values.insert(AirplaneDataKey::Airplane1GpsOnline, m_settings->value(AirplaneDataKey::Airplane1GpsOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane1RadarOnline))
        m_values.insert(AirplaneDataKey::Airplane1RadarOnline, m_settings->value(AirplaneDataKey::Airplane1RadarOnline));

    if(m_settings->contains(AirplaneDataKey::Airplane1ControlState))
        m_values.insert(AirplaneDataKey::Airplane1ControlState, m_settings->value(AirplaneDataKey::Airplane1ControlState));
    if(m_settings->contains(AirplaneDataKey::Airplane1CommunicationChannel))
        m_values.insert(AirplaneDataKey::Airplane1CommunicationChannel, m_settings->value(AirplaneDataKey::Airplane1CommunicationChannel));
    if(m_settings->contains(AirplaneDataKey::Airplane1CommunicationQuality))
        m_values.insert(AirplaneDataKey::Airplane1CommunicationQuality, m_settings->value(AirplaneDataKey::Airplane1CommunicationQuality));

    // 无人机2配置缓存
    if(m_settings->contains(AirplaneDataKey::Airplane2ID))
        m_values.insert(AirplaneDataKey::Airplane2ID, m_settings->value(AirplaneDataKey::Airplane2ID));
    if(m_settings->contains(AirplaneDataKey::Airplane2Name))
        m_values.insert(AirplaneDataKey::Airplane2Name, m_settings->value(AirplaneDataKey::Airplane2Name));
    if(m_settings->contains(AirplaneDataKey::Airplane2Type))
        m_values.insert(AirplaneDataKey::Airplane2Type, m_settings->value(AirplaneDataKey::Airplane2Type));
    if(m_settings->contains(AirplaneDataKey::Airplane2Model))
        m_values.insert(AirplaneDataKey::Airplane2Model, m_settings->value(AirplaneDataKey::Airplane2Model));

    if(m_settings->contains(AirplaneDataKey::Airplane2Latitude))
        m_values.insert(AirplaneDataKey::Airplane2Latitude, m_settings->value(AirplaneDataKey::Airplane2Latitude));
    if(m_settings->contains(AirplaneDataKey::Airplane2Longitude))
        m_values.insert(AirplaneDataKey::Airplane2Longitude, m_settings->value(AirplaneDataKey::Airplane2Longitude));
    if(m_settings->contains(AirplaneDataKey::Airplane2Altitude))
        m_values.insert(AirplaneDataKey::Airplane2Altitude, m_settings->value(AirplaneDataKey::Airplane2Altitude));
    if(m_settings->contains(AirplaneDataKey::Airplane2Heading))
        m_values.insert(AirplaneDataKey::Airplane2Heading, m_settings->value(AirplaneDataKey::Airplane2Heading));
    if(m_settings->contains(AirplaneDataKey::Airplane2Speed))
        m_values.insert(AirplaneDataKey::Airplane2Speed, m_settings->value(AirplaneDataKey::Airplane2Speed));
    if(m_settings->contains(AirplaneDataKey::Airplane2VerticalSpeed))
        m_values.insert(AirplaneDataKey::Airplane2VerticalSpeed, m_settings->value(AirplaneDataKey::Airplane2VerticalSpeed));

    if(m_settings->contains(AirplaneDataKey::Airplane2State))
        m_values.insert(AirplaneDataKey::Airplane2State, m_settings->value(AirplaneDataKey::Airplane2State));
    if(m_settings->contains(AirplaneDataKey::Airplane2FlightDuration))
        m_values.insert(AirplaneDataKey::Airplane2FlightDuration, m_settings->value(AirplaneDataKey::Airplane2FlightDuration));
    if(m_settings->contains(AirplaneDataKey::Airplane2TotalDistance))
        m_values.insert(AirplaneDataKey::Airplane2TotalDistance, m_settings->value(AirplaneDataKey::Airplane2TotalDistance));
    if(m_settings->contains(AirplaneDataKey::Airplane2RemainingOil))
        m_values.insert(AirplaneDataKey::Airplane2RemainingOil, m_settings->value(AirplaneDataKey::Airplane2RemainingOil));
    if(m_settings->contains(AirplaneDataKey::Airplane2SignalStrength))
        m_values.insert(AirplaneDataKey::Airplane2SignalStrength, m_settings->value(AirplaneDataKey::Airplane2SignalStrength));

    if(m_settings->contains(AirplaneDataKey::Airplane2CameraOnline))
        m_values.insert(AirplaneDataKey::Airplane2CameraOnline, m_settings->value(AirplaneDataKey::Airplane2CameraOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane2GpsOnline))
        m_values.insert(AirplaneDataKey::Airplane2GpsOnline, m_settings->value(AirplaneDataKey::Airplane2GpsOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane2RadarOnline))
        m_values.insert(AirplaneDataKey::Airplane2RadarOnline, m_settings->value(AirplaneDataKey::Airplane2RadarOnline));

    if(m_settings->contains(AirplaneDataKey::Airplane2ControlState))
        m_values.insert(AirplaneDataKey::Airplane2ControlState, m_settings->value(AirplaneDataKey::Airplane2ControlState));
    if(m_settings->contains(AirplaneDataKey::Airplane2CommunicationChannel))
        m_values.insert(AirplaneDataKey::Airplane2CommunicationChannel, m_settings->value(AirplaneDataKey::Airplane2CommunicationChannel));
    if(m_settings->contains(AirplaneDataKey::Airplane2CommunicationQuality))
        m_values.insert(AirplaneDataKey::Airplane2CommunicationQuality, m_settings->value(AirplaneDataKey::Airplane2CommunicationQuality));

    // 无人机3配置缓存
    if(m_settings->contains(AirplaneDataKey::Airplane3ID))
        m_values.insert(AirplaneDataKey::Airplane3ID, m_settings->value(AirplaneDataKey::Airplane3ID));
    if(m_settings->contains(AirplaneDataKey::Airplane3Name))
        m_values.insert(AirplaneDataKey::Airplane3Name, m_settings->value(AirplaneDataKey::Airplane3Name));
    if(m_settings->contains(AirplaneDataKey::Airplane3Type))
        m_values.insert(AirplaneDataKey::Airplane3Type, m_settings->value(AirplaneDataKey::Airplane3Type));
    if(m_settings->contains(AirplaneDataKey::Airplane3Model))
        m_values.insert(AirplaneDataKey::Airplane3Model, m_settings->value(AirplaneDataKey::Airplane3Model));

    if(m_settings->contains(AirplaneDataKey::Airplane3Latitude))
        m_values.insert(AirplaneDataKey::Airplane3Latitude, m_settings->value(AirplaneDataKey::Airplane3Latitude));
    if(m_settings->contains(AirplaneDataKey::Airplane3Longitude))
        m_values.insert(AirplaneDataKey::Airplane3Longitude, m_settings->value(AirplaneDataKey::Airplane3Longitude));
    if(m_settings->contains(AirplaneDataKey::Airplane3Altitude))
        m_values.insert(AirplaneDataKey::Airplane3Altitude, m_settings->value(AirplaneDataKey::Airplane3Altitude));
    if(m_settings->contains(AirplaneDataKey::Airplane3Heading))
        m_values.insert(AirplaneDataKey::Airplane3Heading, m_settings->value(AirplaneDataKey::Airplane3Heading));
    if(m_settings->contains(AirplaneDataKey::Airplane3Speed))
        m_values.insert(AirplaneDataKey::Airplane3Speed, m_settings->value(AirplaneDataKey::Airplane3Speed));
    if(m_settings->contains(AirplaneDataKey::Airplane3VerticalSpeed))
        m_values.insert(AirplaneDataKey::Airplane3VerticalSpeed, m_settings->value(AirplaneDataKey::Airplane3VerticalSpeed));

    if(m_settings->contains(AirplaneDataKey::Airplane3State))
        m_values.insert(AirplaneDataKey::Airplane3State, m_settings->value(AirplaneDataKey::Airplane3State));
    if(m_settings->contains(AirplaneDataKey::Airplane3FlightDuration))
        m_values.insert(AirplaneDataKey::Airplane3FlightDuration, m_settings->value(AirplaneDataKey::Airplane3FlightDuration));
    if(m_settings->contains(AirplaneDataKey::Airplane3TotalDistance))
        m_values.insert(AirplaneDataKey::Airplane3TotalDistance, m_settings->value(AirplaneDataKey::Airplane3TotalDistance));
    if(m_settings->contains(AirplaneDataKey::Airplane3RemainingOil))
        m_values.insert(AirplaneDataKey::Airplane3RemainingOil, m_settings->value(AirplaneDataKey::Airplane3RemainingOil));
    if(m_settings->contains(AirplaneDataKey::Airplane3SignalStrength))
        m_values.insert(AirplaneDataKey::Airplane3SignalStrength, m_settings->value(AirplaneDataKey::Airplane3SignalStrength));

    if(m_settings->contains(AirplaneDataKey::Airplane3CameraOnline))
        m_values.insert(AirplaneDataKey::Airplane3CameraOnline, m_settings->value(AirplaneDataKey::Airplane3CameraOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane3GpsOnline))
        m_values.insert(AirplaneDataKey::Airplane3GpsOnline, m_settings->value(AirplaneDataKey::Airplane3GpsOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane3RadarOnline))
        m_values.insert(AirplaneDataKey::Airplane3RadarOnline, m_settings->value(AirplaneDataKey::Airplane3RadarOnline));

    if(m_settings->contains(AirplaneDataKey::Airplane3ControlState))
        m_values.insert(AirplaneDataKey::Airplane3ControlState, m_settings->value(AirplaneDataKey::Airplane3ControlState));
    if(m_settings->contains(AirplaneDataKey::Airplane3CommunicationChannel))
        m_values.insert(AirplaneDataKey::Airplane3CommunicationChannel, m_settings->value(AirplaneDataKey::Airplane3CommunicationChannel));
    if(m_settings->contains(AirplaneDataKey::Airplane3CommunicationQuality))
        m_values.insert(AirplaneDataKey::Airplane3CommunicationQuality, m_settings->value(AirplaneDataKey::Airplane3CommunicationQuality));

    // 无人机4配置缓存
    if(m_settings->contains(AirplaneDataKey::Airplane4ID))
        m_values.insert(AirplaneDataKey::Airplane4ID, m_settings->value(AirplaneDataKey::Airplane4ID));
    if(m_settings->contains(AirplaneDataKey::Airplane4Name))
        m_values.insert(AirplaneDataKey::Airplane4Name, m_settings->value(AirplaneDataKey::Airplane4Name));
    if(m_settings->contains(AirplaneDataKey::Airplane4Type))
        m_values.insert(AirplaneDataKey::Airplane4Type, m_settings->value(AirplaneDataKey::Airplane4Type));
    if(m_settings->contains(AirplaneDataKey::Airplane4Model))
        m_values.insert(AirplaneDataKey::Airplane4Model, m_settings->value(AirplaneDataKey::Airplane4Model));

    if(m_settings->contains(AirplaneDataKey::Airplane4Latitude))
        m_values.insert(AirplaneDataKey::Airplane4Latitude, m_settings->value(AirplaneDataKey::Airplane4Latitude));
    if(m_settings->contains(AirplaneDataKey::Airplane4Longitude))
        m_values.insert(AirplaneDataKey::Airplane4Longitude, m_settings->value(AirplaneDataKey::Airplane4Longitude));
    if(m_settings->contains(AirplaneDataKey::Airplane4Altitude))
        m_values.insert(AirplaneDataKey::Airplane4Altitude, m_settings->value(AirplaneDataKey::Airplane4Altitude));
    if(m_settings->contains(AirplaneDataKey::Airplane4Heading))
        m_values.insert(AirplaneDataKey::Airplane4Heading, m_settings->value(AirplaneDataKey::Airplane4Heading));
    if(m_settings->contains(AirplaneDataKey::Airplane4Speed))
        m_values.insert(AirplaneDataKey::Airplane4Speed, m_settings->value(AirplaneDataKey::Airplane4Speed));
    if(m_settings->contains(AirplaneDataKey::Airplane4VerticalSpeed))
        m_values.insert(AirplaneDataKey::Airplane4VerticalSpeed, m_settings->value(AirplaneDataKey::Airplane4VerticalSpeed));

    if(m_settings->contains(AirplaneDataKey::Airplane4State))
        m_values.insert(AirplaneDataKey::Airplane4State, m_settings->value(AirplaneDataKey::Airplane4State));
    if(m_settings->contains(AirplaneDataKey::Airplane4FlightDuration))
        m_values.insert(AirplaneDataKey::Airplane4FlightDuration, m_settings->value(AirplaneDataKey::Airplane4FlightDuration));
    if(m_settings->contains(AirplaneDataKey::Airplane4TotalDistance))
        m_values.insert(AirplaneDataKey::Airplane4TotalDistance, m_settings->value(AirplaneDataKey::Airplane4TotalDistance));
    if(m_settings->contains(AirplaneDataKey::Airplane4RemainingOil))
        m_values.insert(AirplaneDataKey::Airplane4RemainingOil, m_settings->value(AirplaneDataKey::Airplane4RemainingOil));
    if(m_settings->contains(AirplaneDataKey::Airplane4SignalStrength))
        m_values.insert(AirplaneDataKey::Airplane4SignalStrength, m_settings->value(AirplaneDataKey::Airplane4SignalStrength));

    if(m_settings->contains(AirplaneDataKey::Airplane4CameraOnline))
        m_values.insert(AirplaneDataKey::Airplane4CameraOnline, m_settings->value(AirplaneDataKey::Airplane4CameraOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane4GpsOnline))
        m_values.insert(AirplaneDataKey::Airplane4GpsOnline, m_settings->value(AirplaneDataKey::Airplane4GpsOnline));
    if(m_settings->contains(AirplaneDataKey::Airplane4RadarOnline))
        m_values.insert(AirplaneDataKey::Airplane4RadarOnline, m_settings->value(AirplaneDataKey::Airplane4RadarOnline));

    if(m_settings->contains(AirplaneDataKey::Airplane4ControlState))
        m_values.insert(AirplaneDataKey::Airplane4ControlState, m_settings->value(AirplaneDataKey::Airplane4ControlState));
    if(m_settings->contains(AirplaneDataKey::Airplane4CommunicationChannel))
        m_values.insert(AirplaneDataKey::Airplane4CommunicationChannel, m_settings->value(AirplaneDataKey::Airplane4CommunicationChannel));
    if(m_settings->contains(AirplaneDataKey::Airplane4CommunicationQuality))
        m_values.insert(AirplaneDataKey::Airplane4CommunicationQuality, m_settings->value(AirplaneDataKey::Airplane4CommunicationQuality));

    // 无人机1装订点缓存
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint1Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint1Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint1Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint1Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint1Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint1Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint2Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint2Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint2Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint2Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint2Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint2Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint3Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint3Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint3Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint3Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint3Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint3Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint4Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint4Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint4Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint4Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint4Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint4Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint5Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint5Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint5Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint5Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint5Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint5Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint6Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint6Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint6Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint6Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint6Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint6Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint7Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint7Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint7Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint7Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint7Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint7Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint8Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint8Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint8Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint8Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint8Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint8Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint9Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint9Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint9Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint9Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint9Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint9Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint10Lat))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint10Lat, m_settings->value(AirplaneDataKey::Airplane1WayPoint10Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane1WayPoint10Lon))
        m_values.insert(AirplaneDataKey::Airplane1WayPoint10Lon, m_settings->value(AirplaneDataKey::Airplane1WayPoint10Lon));

    // 无人机2装订点缓存
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint1Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint1Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint1Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint1Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint1Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint1Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint2Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint2Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint2Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint2Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint2Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint2Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint3Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint3Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint3Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint3Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint3Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint3Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint4Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint4Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint4Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint4Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint4Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint4Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint5Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint5Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint5Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint5Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint5Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint5Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint6Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint6Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint6Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint6Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint6Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint6Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint7Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint7Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint7Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint7Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint7Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint7Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint8Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint8Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint8Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint8Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint8Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint8Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint9Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint9Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint9Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint9Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint9Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint9Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint10Lat))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint10Lat, m_settings->value(AirplaneDataKey::Airplane2WayPoint10Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane2WayPoint10Lon))
        m_values.insert(AirplaneDataKey::Airplane2WayPoint10Lon, m_settings->value(AirplaneDataKey::Airplane2WayPoint10Lon));

    // 无人机3装订点缓存
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint1Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint1Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint1Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint1Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint1Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint1Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint2Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint2Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint2Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint2Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint2Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint2Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint3Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint3Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint3Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint3Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint3Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint3Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint4Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint4Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint4Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint4Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint4Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint4Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint5Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint5Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint5Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint5Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint5Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint5Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint6Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint6Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint6Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint6Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint6Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint6Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint7Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint7Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint7Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint7Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint7Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint7Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint8Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint8Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint8Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint8Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint8Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint8Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint9Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint9Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint9Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint9Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint9Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint9Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint10Lat))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint10Lat, m_settings->value(AirplaneDataKey::Airplane3WayPoint10Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane3WayPoint10Lon))
        m_values.insert(AirplaneDataKey::Airplane3WayPoint10Lon, m_settings->value(AirplaneDataKey::Airplane3WayPoint10Lon));


    // 无人机4装订点缓存
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint1Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint1Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint1Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint1Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint1Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint1Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint2Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint2Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint2Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint2Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint2Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint2Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint3Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint3Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint3Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint3Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint3Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint3Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint4Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint4Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint4Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint4Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint4Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint4Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint5Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint5Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint5Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint5Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint5Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint5Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint6Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint6Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint6Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint6Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint6Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint6Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint7Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint7Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint7Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint7Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint7Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint7Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint8Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint8Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint8Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint8Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint8Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint8Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint9Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint9Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint9Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint9Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint9Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint9Lon));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint10Lat))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint10Lat, m_settings->value(AirplaneDataKey::Airplane4WayPoint10Lat));
    if(m_settings->contains(AirplaneDataKey::Airplane4WayPoint10Lon))
        m_values.insert(AirplaneDataKey::Airplane4WayPoint10Lon, m_settings->value(AirplaneDataKey::Airplane4WayPoint10Lon));
}

void AirplaneDatas::saveSettings()
{
    // Write all cached values back to settings
    QHash<QString, QVariant>::const_iterator it;
    for (it = m_values.constBegin(); it != m_values.constEnd(); ++it) {
        m_settings->setValue(it.key(), it.value());
    }
    
    // Sync settings to ensure data is written to file
    m_settings->sync();
}

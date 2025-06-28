#ifndef AIRPLANEDATAS_H
#define AIRPLANEDATAS_H

#include <QSettings>
#include <QObject>
#include <QGVGlobal.h>

namespace AirplaneDataKey {
    //simulation - UAV configuration parameters
    // Basic information configuration
    const QString Airplane1ID = "airplane1_id";
    const QString Airplane1Name = "airplane1_name";
    const QString Airplane1Type = "airplane1_type";
    const QString Airplane1Model = "airplane1_model";

    const QString Airplane2ID = "airplane2_id";
    const QString Airplane2Name = "airplane2_name";
    const QString Airplane2Type = "airplane2_type";
    const QString Airplane2Model = "airplane2_model";

    const QString Airplane3ID = "airplane3_id";
    const QString Airplane3Name = "airplane3_name";
    const QString Airplane3Type = "airplane3_type";
    const QString Airplane3Model = "airplane3_model";

    const QString Airplane4ID = "airplane4_id";
    const QString Airplane4Name = "airplane4_name";
    const QString Airplane4Type = "airplane4_type";
    const QString Airplane4Model = "airplane4_model";

    // Position information configuration
    const QString Airplane1Latitude = "airplane1_latitude";
    const QString Airplane1Longitude = "airplane1_longitude";
    const QString Airplane1Altitude = "airplane1_altitude";
    const QString Airplane1Heading = "airplane1_heading";
    const QString Airplane1Speed = "airplane1_speed";
    const QString Airplane1VerticalSpeed = "airplane1_vertical_speed";

    const QString Airplane2Latitude = "airplane2_latitude";
    const QString Airplane2Longitude = "airplane2_longitude";
    const QString Airplane2Altitude = "airplane2_altitude";
    const QString Airplane2Heading = "airplane2_heading";
    const QString Airplane2Speed = "airplane2_speed";
    const QString Airplane2VerticalSpeed = "airplane2_vertical_speed";

    const QString Airplane3Latitude = "airplane3_latitude";
    const QString Airplane3Longitude = "airplane3_longitude";
    const QString Airplane3Altitude = "airplane3_altitude";
    const QString Airplane3Heading = "airplane3_heading";
    const QString Airplane3Speed = "airplane3_speed";
    const QString Airplane3VerticalSpeed = "airplane3_vertical_speed";

    const QString Airplane4Latitude = "airplane4_latitude";
    const QString Airplane4Longitude = "airplane4_longitude";
    const QString Airplane4Altitude = "airplane4_altitude";
    const QString Airplane4Heading = "airplane4_heading";
    const QString Airplane4Speed = "airplane4_speed";
    const QString Airplane4VerticalSpeed = "airplane4_vertical_speed";

    // Status information configuration
    const QString Airplane1State = "airplane1_state";
    const QString Airplane2State = "airplane2_state";
    const QString Airplane3State = "airplane3_state";
    const QString Airplane4State = "airplane4_state";

    // Flight information configuration
    const QString Airplane1FlightDuration = "airplane1_flight_duration";
    const QString Airplane1TotalDistance = "airplane1_total_distance";
    const QString Airplane1RemainingOil = "airplane1_remaining_oil";
    const QString Airplane1SignalStrength = "airplane1_signal_strength";

    const QString Airplane2FlightDuration = "airplane2_flight_duration";
    const QString Airplane2TotalDistance = "airplane2_total_distance";
    const QString Airplane2RemainingOil = "airplane2_remaining_oil";
    const QString Airplane2SignalStrength = "airplane2_signal_strength";

    const QString Airplane3FlightDuration = "airplane3_flight_duration";
    const QString Airplane3TotalDistance = "airplane3_total_distance";
    const QString Airplane3RemainingOil = "airplane3_remaining_oil";
    const QString Airplane3SignalStrength = "airplane3_signal_strength";

    const QString Airplane4FlightDuration = "airplane4_flight_duration";
    const QString Airplane4TotalDistance = "airplane4_total_distance";
    const QString Airplane4RemainingOil = "airplane4_remaining_oil";
    const QString Airplane4SignalStrength = "airplane4_signal_strength";

    // Sensor information configuration
    const QString Airplane1CameraOnline = "airplane1_camera_online";
    const QString Airplane1GpsOnline = "airplane1_gps_online";
    const QString Airplane1RadarOnline = "airplane1_radar_online";

    const QString Airplane2CameraOnline = "airplane2_camera_online";
    const QString Airplane2GpsOnline = "airplane2_gps_online";
    const QString Airplane2RadarOnline = "airplane2_radar_online";

    const QString Airplane3CameraOnline = "airplane3_camera_online";
    const QString Airplane3GpsOnline = "airplane3_gps_online";
    const QString Airplane3RadarOnline = "airplane3_radar_online";

    const QString Airplane4CameraOnline = "airplane4_camera_online";
    const QString Airplane4GpsOnline = "airplane4_gps_online";
    const QString Airplane4RadarOnline = "airplane4_radar_online";

    // Control information configuration
    const QString Airplane1ControlState = "airplane1_control_state";
    const QString Airplane2ControlState = "airplane2_control_state";
    const QString Airplane3ControlState = "airplane3_control_state";
    const QString Airplane4ControlState = "airplane4_control_state";

    // Communication information configuration
    const QString Airplane1CommunicationChannel = "airplane1_communication_channel";
    const QString Airplane1CommunicationQuality = "airplane1_communication_quality";

    const QString Airplane2CommunicationChannel = "airplane2_communication_channel";
    const QString Airplane2CommunicationQuality = "airplane2_communication_quality";

    const QString Airplane3CommunicationChannel = "airplane3_communication_channel";
    const QString Airplane3CommunicationQuality = "airplane3_communication_quality";

    const QString Airplane4CommunicationChannel = "airplane4_communication_channel";
    const QString Airplane4CommunicationQuality = "airplane4_communication_quality";

    // Flight path waypoint configuration - maximum 10 waypoints per UAV
    // UAV 1 waypoints
    const QString Airplane1WayPoint1Lat = "airplane1_waypoint1_lat";
    const QString Airplane1WayPoint1Lon = "airplane1_waypoint1_lon";
    const QString Airplane1WayPoint2Lat = "airplane1_waypoint2_lat";
    const QString Airplane1WayPoint2Lon = "airplane1_waypoint2_lon";
    const QString Airplane1WayPoint3Lat = "airplane1_waypoint3_lat";
    const QString Airplane1WayPoint3Lon = "airplane1_waypoint3_lon";
    const QString Airplane1WayPoint4Lat = "airplane1_waypoint4_lat";
    const QString Airplane1WayPoint4Lon = "airplane1_waypoint4_lon";
    const QString Airplane1WayPoint5Lat = "airplane1_waypoint5_lat";
    const QString Airplane1WayPoint5Lon = "airplane1_waypoint5_lon";
    const QString Airplane1WayPoint6Lat = "airplane1_waypoint6_lat";
    const QString Airplane1WayPoint6Lon = "airplane1_waypoint6_lon";
    const QString Airplane1WayPoint7Lat = "airplane1_waypoint7_lat";
    const QString Airplane1WayPoint7Lon = "airplane1_waypoint7_lon";
    const QString Airplane1WayPoint8Lat = "airplane1_waypoint8_lat";
    const QString Airplane1WayPoint8Lon = "airplane1_waypoint8_lon";
    const QString Airplane1WayPoint9Lat = "airplane1_waypoint9_lat";
    const QString Airplane1WayPoint9Lon = "airplane1_waypoint9_lon";
    const QString Airplane1WayPoint10Lat = "airplane1_waypoint10_lat";
    const QString Airplane1WayPoint10Lon = "airplane1_waypoint10_lon";

    // UAV 2 waypoints
    const QString Airplane2WayPoint1Lat = "airplane2_waypoint1_lat";
    const QString Airplane2WayPoint1Lon = "airplane2_waypoint1_lon";
    const QString Airplane2WayPoint2Lat = "airplane2_waypoint2_lat";
    const QString Airplane2WayPoint2Lon = "airplane2_waypoint2_lon";
    const QString Airplane2WayPoint3Lat = "airplane2_waypoint3_lat";
    const QString Airplane2WayPoint3Lon = "airplane2_waypoint3_lon";
    const QString Airplane2WayPoint4Lat = "airplane2_waypoint4_lat";
    const QString Airplane2WayPoint4Lon = "airplane2_waypoint4_lon";
    const QString Airplane2WayPoint5Lat = "airplane2_waypoint5_lat";
    const QString Airplane2WayPoint5Lon = "airplane2_waypoint5_lon";
    const QString Airplane2WayPoint6Lat = "airplane2_waypoint6_lat";
    const QString Airplane2WayPoint6Lon = "airplane2_waypoint6_lon";
    const QString Airplane2WayPoint7Lat = "airplane2_waypoint7_lat";
    const QString Airplane2WayPoint7Lon = "airplane2_waypoint7_lon";
    const QString Airplane2WayPoint8Lat = "airplane2_waypoint8_lat";
    const QString Airplane2WayPoint8Lon = "airplane2_waypoint8_lon";
    const QString Airplane2WayPoint9Lat = "airplane2_waypoint9_lat";
    const QString Airplane2WayPoint9Lon = "airplane2_waypoint9_lon";
    const QString Airplane2WayPoint10Lat = "airplane2_waypoint10_lat";
    const QString Airplane2WayPoint10Lon = "airplane2_waypoint10_lon";

    // UAV 3 waypoints
    const QString Airplane3WayPoint1Lat = "airplane3_waypoint1_lat";
    const QString Airplane3WayPoint1Lon = "airplane3_waypoint1_lon";
    const QString Airplane3WayPoint2Lat = "airplane3_waypoint2_lat";
    const QString Airplane3WayPoint2Lon = "airplane3_waypoint2_lon";
    const QString Airplane3WayPoint3Lat = "airplane3_waypoint3_lat";
    const QString Airplane3WayPoint3Lon = "airplane3_waypoint3_lon";
    const QString Airplane3WayPoint4Lat = "airplane3_waypoint4_lat";
    const QString Airplane3WayPoint4Lon = "airplane3_waypoint4_lon";
    const QString Airplane3WayPoint5Lat = "airplane3_waypoint5_lat";
    const QString Airplane3WayPoint5Lon = "airplane3_waypoint5_lon";
    const QString Airplane3WayPoint6Lat = "airplane3_waypoint6_lat";
    const QString Airplane3WayPoint6Lon = "airplane3_waypoint6_lon";
    const QString Airplane3WayPoint7Lat = "airplane3_waypoint7_lat";
    const QString Airplane3WayPoint7Lon = "airplane3_waypoint7_lon";
    const QString Airplane3WayPoint8Lat = "airplane3_waypoint8_lat";
    const QString Airplane3WayPoint8Lon = "airplane3_waypoint8_lon";
    const QString Airplane3WayPoint9Lat = "airplane3_waypoint9_lat";
    const QString Airplane3WayPoint9Lon = "airplane3_waypoint9_lon";
    const QString Airplane3WayPoint10Lat = "airplane3_waypoint10_lat";
    const QString Airplane3WayPoint10Lon = "airplane3_waypoint10_lon";

    const QString Airplane4WayPoint1Lat = "airplane4_waypoint1_lat";
    const QString Airplane4WayPoint1Lon = "airplane4_waypoint1_lon";
    const QString Airplane4WayPoint2Lat = "airplane4_waypoint2_lat";
    const QString Airplane4WayPoint2Lon = "airplane4_waypoint2_lon";
    const QString Airplane4WayPoint3Lat = "airplane4_waypoint3_lat";
    const QString Airplane4WayPoint3Lon = "airplane4_waypoint3_lon";
    const QString Airplane4WayPoint4Lat = "airplane4_waypoint4_lat";
    const QString Airplane4WayPoint4Lon = "airplane4_waypoint4_lon";
    const QString Airplane4WayPoint5Lat = "airplane4_waypoint5_lat";
    const QString Airplane4WayPoint5Lon = "airplane4_waypoint5_lon";
    const QString Airplane4WayPoint6Lat = "airplane4_waypoint6_lat";
    const QString Airplane4WayPoint6Lon = "airplane4_waypoint6_lon";
    const QString Airplane4WayPoint7Lat = "airplane4_waypoint7_lat";
    const QString Airplane4WayPoint7Lon = "airplane4_waypoint7_lon";
    const QString Airplane4WayPoint8Lat = "airplane4_waypoint8_lat";
    const QString Airplane4WayPoint8Lon = "airplane4_waypoint8_lon";
    const QString Airplane4WayPoint9Lat = "airplane4_waypoint9_lat";
    const QString Airplane4WayPoint9Lon = "airplane4_waypoint9_lon";
    const QString Airplane4WayPoint10Lat = "airplane4_waypoint10_lat";
    const QString Airplane4WayPoint10Lon = "airplane4_waypoint10_lon";
};

class AirplaneDatas : public QObject
{
    Q_OBJECT
public:
    explicit AirplaneDatas(QObject *parent = nullptr);
    ~AirplaneDatas();

    QList<QGV::GeoPos> getWayPoints(int airplaneIndex);
    QVariant getValue(const QString key) const;
    void setValue(const QString &key, const QVariant &value);
    void saveSettings();
private:
    void initSettings();
    void cacheSettings();
    QHash<QString, QVariant> m_values;
    QSettings *m_settings;
};

#endif // AIRPLANEDATAS_H

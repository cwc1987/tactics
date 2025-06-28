#pragma once

#include <QGVDrawItem.h>
#include <QGVGlobal.h>
#include <QPainter>
#include <QPolygonF>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QColor>
#include "airplanemodel.h"

// Forward declaration
class AirPlaneItem;

class Airplane : public QGVDrawItem
{
    Q_OBJECT

public:
    explicit Airplane();
    explicit Airplane(const AirPlaneItem& airplaneItem);
    explicit Airplane(const QGV::GeoPos& geoPos, const double &heading = 0.0f, const AirPlaneItem::AirState &state = AirPlaneItem::Normal);
    ~Airplane();

    // Set airplane data from AirPlaneItem
    void setAirplaneData(const AirPlaneItem& airplaneItem);

    // Getters
    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getType() const { return m_type; }
    QString getModel() const { return m_model; }
    QGV::GeoPos position() const { return m_geoPos; }
    double getAltitude() const { return m_altitude; }
    double heading() const { return m_heading; }
    double getSpeed() const { return m_speed; }
    double getVerticalSpeed() const { return m_verticalSpeed; }
    AirPlaneItem::AirState getState() const { return m_state; }
    AirPlaneItem::AirControlState getControlState() const { return m_controlState; }
    double getRemainingOil() const { return m_remainingOil; }
    double getSignalStrength() const { return m_signalStrength; }
    bool isCameraOnline() const { return m_cameraOnline; }
    bool isGpsOnline() const { return m_gpsOnline; }
    bool isRadarOnline() const { return m_radarOnline; }
    double getCommunicationQuality() const { return m_communicationQuality; }
    QDateTime getLastCommunication() const { return m_lastCommunication; }
    bool isActive() const { return m_isActive; }

    // QGVDrawItem interface implementation
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;
    QString projTooltip(const QPointF& projPos) const override;

signals:
    void signalPositionChanged(QGV::GeoPos position);

protected:
    void onProjection(QGVMap* geoMap) override;
    QPointF projAnchor() const override;
    QTransform projTransform() const override;

private:
    // Airplane properties
    QString m_id;
    QString m_name;
    QString m_type;
    QString m_model;
    QGV::GeoPos m_geoPos;
    QPointF m_projPos;
    double m_altitude;
    double m_heading;
    double m_speed;
    double m_verticalSpeed;
    AirPlaneItem::AirState m_state;
    AirPlaneItem::AirControlState m_controlState;
    double m_remainingOil;
    double m_signalStrength;
    bool m_cameraOnline;
    bool m_gpsOnline;
    bool m_radarOnline;
    double m_communicationQuality;
    QDateTime m_lastCommunication;
    bool m_isActive;
    
    // Display properties
    QImage m_airNormalImage;
    QImage m_airUnknownImage;
    QImage m_airDamageImage;
    QSizeF m_airIconSize;
    
    // Cached painter path for performance
    mutable QPainterPath m_cachedPath;
    
    // Helper methods
    void updatePath() const;
    QColor getDisplayColor() const;
    QString getStateString() const;
    QString getControlStateString() const;
    QPen getAirplanePen() const;
    QBrush getAirplaneBrush() const;
    void loadImages();
    QColor getTextColorForBackground(const QColor &background) const;
}; 

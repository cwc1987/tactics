#ifndef ARMOREDCAR_H
#define ARMOREDCAR_H

#include <QGVDrawItem.h>
#include <QGVGlobal.h>
#include <QPainter>
#include <QPolygonF>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QColor>
#include "targetmodel.h"

// Forward declaration
class TargetItem;

class ArmoredCar : public QGVDrawItem
{
    Q_OBJECT

public:
    explicit ArmoredCar();
    explicit ArmoredCar(const TargetItem& targetItem);
    explicit ArmoredCar(const QGV::GeoPos& geoPos, const double &heading = 0.0f,
                       const TargetItem::TargetState &state = TargetItem::Normal);
    ~ArmoredCar();

    // Set armored car data from TargetItem
    void setArmoredCarData(const TargetItem& targetItem);

    // Getters
    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    QString getModel() const { return m_model; }
    TargetItem::TargetType getType() const { return m_type; }
    QGV::GeoPos position() const { return m_geoPos; }
    double getAltitude() const { return m_altitude; }
    double heading() const { return m_heading; }
    double getSpeed() const { return m_speed; }
    double getVerticalSpeed() const { return m_verticalSpeed; }
    TargetItem::TargetState getState() const { return m_state; }
    TargetItem::ThreatLevel getThreatLevel() const { return m_threatLevel; }
    double getConfidence() const { return m_confidence; }
    bool isDetected() const { return m_isDetected; }
    bool isTracked() const { return m_isTracked; }
    double getDetectionRange() const { return m_detectionRange; }
    double getWeaponRange() const { return m_weaponRange; }
    int getMissileCount() const { return m_missileCount; }
    QString getCommunicationType() const { return m_communicationType; }
    double getCommunicationRange() const { return m_communicationRange; }
    QDateTime getFirstDetectionTime() const { return m_firstDetectionTime; }
    QDateTime getLastDetectionTime() const { return m_lastDetectionTime; }
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
    // Armored car properties
    QString m_id;
    QString m_name;
    QString m_model;
    TargetItem::TargetType m_type;
    QGV::GeoPos m_geoPos;
    QPointF m_projPos;
    double m_altitude;
    double m_heading;
    double m_speed;
    double m_verticalSpeed;
    TargetItem::TargetState m_state;
    TargetItem::ThreatLevel m_threatLevel;
    double m_confidence;
    bool m_isDetected;
    bool m_isTracked;
    double m_detectionRange;
    double m_weaponRange;
    int m_missileCount;
    QString m_communicationType;
    double m_communicationRange;
    QDateTime m_firstDetectionTime;
    QDateTime m_lastDetectionTime;
    bool m_isActive;
    
    // Display properties
    QImage m_armoredCarNormalImage;
    QImage m_armoredCarDamageImage;
    QImage m_armoredCarDestroyedImage;
    QImage m_armoredCarUnknownImage;
    QSizeF m_armoredCarIconSize;
    
    // Cached painter path for performance
    mutable QPainterPath m_cachedPath;
    
    // Helper methods
    void updatePath() const;
    QColor getDisplayColor() const;
    QString getStateString() const;
    QString getThreatLevelString() const;
    QString getTypeString() const;
    QPen getArmoredCarPen() const;
    QBrush getArmoredCarBrush() const;
    void loadImages();
    QColor getTextColorForBackground(const QColor &background) const;
}; 

#endif // ARMOREDCAR_H

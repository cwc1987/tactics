#ifndef AREA_H
#define AREA_H

#include <QGVDrawItem.h>
#include <QPainterPath>
#include <QColor>
#include <QString>
#include <QList>
#include <QPointF>

// Forward declaration
class AreaItem;
class Area : public QGVDrawItem
{
    Q_OBJECT
public:
    explicit Area();
    explicit Area(const AreaItem& areaItem);
    
    // Set area data from AreaItem
    void setAreaData(const AreaItem& areaItem);
    
    // Getters
    QString getId() const { return m_id; }
    QString getName() const { return m_name; }
    int getAreaType() const { return m_areaType; }
    QString getDescription() const { return m_description; }
    QString getColor() const { return m_color; }
    double getMinAltitude() const { return m_minAltitude; }
    double getMaxAltitude() const { return m_maxAltitude; }
    bool isActive() const { return m_isActive; }
    QList<QGV::GeoPos> getCoordinates() const { return m_coordinates; }
    
    // QGVDrawItem interface implementation
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;
    QString projTooltip(const QPointF& projPos) const override;

private:
    // Area properties
    QString m_id;
    QString m_name;
    int m_areaType;  // 0: TaskArea, 1: NoFlyZone, 2: ThreatZone
    QString m_description;
    QString m_color;
    double m_minAltitude;
    double m_maxAltitude;
    bool m_isActive;
    
    // Coordinates for area boundary
    QList<QGV::GeoPos> m_coordinates;
    
    // Cached painter path for performance
    mutable QPainterPath m_cachedPath;
    mutable bool m_pathDirty;
    
    // Helper methods
    void updatePath() const;
    QColor getDisplayColor() const;
    QString getAreaTypeString() const;
    QPen getAreaPen() const;
    QBrush getAreaBrush() const;
};

#endif // AREA_H

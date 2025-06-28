#ifndef AIRPLANETRACK_H
#define AIRPLANETRACK_H

#include <QQueue>
#include <QGeoView/QGVDrawItem.h>
#include <QGeoView/QGVGlobal.h>
#include <QObject>
#include "airplane.h"

class AirplaneTrack : public QGVDrawItem
{
    Q_OBJECT
public:
    explicit AirplaneTrack(Airplane *airplane);
    ~AirplaneTrack();

    Airplane *getAirplane();
public slots:
    void onAirplanePositionChanged(QGV::GeoPos position);

protected:
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;

private:
    QQueue<QGV::GeoPos> m_tracks;
    QVector<QPointF> m_projTracks;
    Airplane *m_airplane;
    int m_trackSize;
    int m_trackWidth;
    QColor m_lineColor;
};

#endif // AIRPLANETRACK_H

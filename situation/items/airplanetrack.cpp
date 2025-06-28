#include "airplanetrack.h"
#include <QTransform>
#include "settings.h"

AirplaneTrack::AirplaneTrack(Airplane *airplane)
    : m_airplane(airplane)
{
    m_trackSize = Settings::instance()->getValue(SettingsKey::AirTrackSize).toInt();
    m_trackWidth = Settings::instance()->getValue(SettingsKey::AirTrackWidth).toInt();
    m_lineColor = Settings::instance()->getColor(SettingsKey::AirTrackColor);
    m_tracks.enqueue(m_airplane->position());
    connect(airplane, &Airplane::signalPositionChanged,
            this, &AirplaneTrack::onAirplanePositionChanged);
}

AirplaneTrack::~AirplaneTrack()
{
}

Airplane *AirplaneTrack::getAirplane()
{
    return m_airplane;
}

void AirplaneTrack::onAirplanePositionChanged(QGV::GeoPos position)
{
    m_tracks.enqueue(position);
    if(m_tracks.size() > m_trackSize)
        m_tracks.dequeue();

    onProjection(getMap());
    resetBoundary();
    refresh();
}

void AirplaneTrack::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    m_projTracks.clear();
    m_projTracks.resize(m_tracks.size());
    for(int i = 0, iend = m_tracks.count(); i < iend; i++) {
        QPointF projPos = geoMap->getProjection()->geoToProj(m_tracks[i]);
        m_projTracks[i] = projPos;
    }
}

QPainterPath AirplaneTrack::projShape() const
{
    QPainterPath path;
    QPolygonF projPolygon(m_projTracks);
    path.addPolygon(projPolygon);
    return path;
}

void AirplaneTrack::projPaint(QPainter *painter)
{
    if (!painter)
        return;

    if(m_projTracks.size() < 1)
        return;

    painter->save();
    QPen pen(m_lineColor);
    const QGVCameraState state = getMap()->getCamera();
    double penWidth = (double)m_trackWidth / state.scale();
    pen.setWidthF(penWidth);
    painter->setPen(pen);

    QPolygonF projPolygon(m_projTracks);
    painter->drawPolygon(projPolygon);

    painter->restore();
}

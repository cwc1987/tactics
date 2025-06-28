#include "airplane.h"
#include <QGVMap.h>
#include <QGVProjection.h>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QtMath>
#include <QDebug>
#include "settings.h"

Airplane::Airplane()
    : m_altitude(0.0)
    , m_heading(0.0)
    , m_speed(0.0)
    , m_verticalSpeed(0.0)
    , m_state(AirPlaneItem::Normal)
    , m_controlState(AirPlaneItem::Manual)
    , m_remainingOil(100.0)
    , m_signalStrength(100.0)
    , m_cameraOnline(false)
    , m_gpsOnline(false)
    , m_radarOnline(false)
    , m_communicationQuality(100.0)
    , m_isActive(false)
{
    loadImages();
    setFlag(QGV::ItemFlag::Transformed);
}

Airplane::Airplane(const AirPlaneItem& airplaneItem)
    : m_altitude(0.0)
    , m_heading(0.0)
    , m_speed(0.0)
    , m_verticalSpeed(0.0)
    , m_state(AirPlaneItem::Normal)
    , m_controlState(AirPlaneItem::Manual)
    , m_remainingOil(100.0)
    , m_signalStrength(100.0)
    , m_cameraOnline(false)
    , m_gpsOnline(false)
    , m_radarOnline(false)
    , m_communicationQuality(100.0)
    , m_isActive(false)
{
    setAirplaneData(airplaneItem);
    loadImages();
    setFlag(QGV::ItemFlag::Transformed);
}

Airplane::Airplane(const QGV::GeoPos& geoPos, const double &heading, const AirPlaneItem::AirState &state)
    : m_geoPos(geoPos)
    , m_altitude(0.0)
    , m_heading(heading)
    , m_speed(0.0)
    , m_verticalSpeed(0.0)
    , m_state(state)
    , m_controlState(AirPlaneItem::Manual)
    , m_remainingOil(100.0)
    , m_signalStrength(100.0)
    , m_cameraOnline(false)
    , m_gpsOnline(false)
    , m_radarOnline(false)
    , m_communicationQuality(100.0)
    , m_isActive(false)
{
    loadImages();
    setFlag(QGV::ItemFlag::Transformed);
}

Airplane::~Airplane()
{
}

void Airplane::setAirplaneData(const AirPlaneItem& airplaneItem)
{
    bool updatePositon = false;
    m_id = airplaneItem.id;
    m_name = airplaneItem.name;
    m_type = airplaneItem.type;
    m_model = airplaneItem.model;
    if(!qFuzzyCompare(m_geoPos.latitude(), airplaneItem.latitude) ||
        !qFuzzyCompare(m_geoPos.longitude(), airplaneItem.longitude)) {
        updatePositon = true;
    }
    m_geoPos = QGV::GeoPos(airplaneItem.latitude, airplaneItem.longitude);
    m_altitude = airplaneItem.altitude;
    m_heading = airplaneItem.heading;
    m_speed = airplaneItem.speed;
    m_verticalSpeed = airplaneItem.verticalSpeed;
    m_state = airplaneItem.state;
    m_controlState = airplaneItem.controlState;
    m_remainingOil = airplaneItem.remainingOil;
    m_signalStrength = airplaneItem.signalStrength;
    m_cameraOnline = airplaneItem.cameraOnline;
    m_gpsOnline = airplaneItem.gpsOnline;
    m_radarOnline = airplaneItem.radarOnline;
    m_communicationQuality = airplaneItem.communicationQuality;
    m_lastCommunication = airplaneItem.lastCommunication;
    m_isActive = airplaneItem.isActive;
    if(updatePositon) {
        emit signalPositionChanged(m_geoPos);
    }
    onProjection(getMap());
    resetBoundary();
    refresh();
}

void Airplane::onProjection(QGVMap *geoMap)
{
    if(geoMap == nullptr)
        return;

    QGVDrawItem::onProjection(geoMap);
    m_projPos = geoMap->getProjection()->geoToProj(m_geoPos);
}

QPainterPath Airplane::projShape() const
{
    updatePath();
    return m_cachedPath;
}

void Airplane::projPaint(QPainter* painter)
{
    if (!painter) {
        return;
    }
    
    updatePath();
    
    // Set up painter
    //painter->setRenderHint(QPainter::Antialiasing, true);
    
    const QGVCameraState state = getMap()->getCamera();
    double wvalidSize = m_airIconSize.width() / state.scale();
    double hvalidSize = m_airIconSize.height() / state.scale();
    QRectF projRect(m_projPos - QPointF(wvalidSize/2, hvalidSize/2),
                    m_projPos + QPointF(wvalidSize/2, hvalidSize/2));

    // Draw airplane image based on state
    if(m_state == AirPlaneItem::Normal)
        painter->drawImage(projRect, m_airNormalImage);
    else if(m_state == AirPlaneItem::Damage)
        painter->drawImage(projRect, m_airDamageImage);
    else
        painter->drawImage(projRect, m_airUnknownImage);
    
    // Draw airplane label if name is not empty and active
    // if (!m_name.isEmpty() && m_isActive) {
    //     // Set font
    //     QFont font = painter->font();
    //     font.setPointSizeF(8.0f / state.scale());
    //     painter->setFont(font);
        
    //     // Calculate text position (above the airplane)
    //     QPointF textPos = m_projPos + QPointF(0, -hvalidSize/2 - 5.0f / state.scale());
        
    //     // Calculate background color and text color
    //     QColor bgColor = getDisplayColor();
    //     bgColor.setAlpha(180); // Semi-transparent background
        
    //     double luminance = 0.299 * bgColor.red() + 0.587 * bgColor.green() + 0.114 * bgColor.blue();
    //     QColor textColor = (luminance > 140) ? Qt::black : Qt::white;
        
    //     // Calculate text rectangle
    //     QFontMetrics fm(font);
    //     QRect textRect = fm.boundingRect(m_name);
    //     textRect.moveCenter(textPos.toPoint());
        
    //     // Draw background rectangle
    //     painter->setBrush(bgColor);
    //     painter->setPen(Qt::NoPen);
    //     painter->drawRect(textRect);
        
    //     // Draw text
    //     painter->setPen(textColor);
    //     painter->drawText(textRect, Qt::AlignCenter, m_name);
    // }
}

QString Airplane::projTooltip(const QPointF& projPos) const
{
    Q_UNUSED(projPos)
    
    QString tooltip = QString(tr("Airplane: %1\nType: %2\nModel: %3\nState: %4\nControl: %5\nAltitude: %6 m\nSpeed: %7 m/s\nOil: %8%\nSignal: %9%"))
                     .arg(m_name.isEmpty() ? m_id : m_name)
                     .arg(m_type.isEmpty() ? tr("Unknown") : m_type)
                     .arg(m_model.isEmpty() ? tr("Unknown") : m_model)
                     .arg(getStateString())
                     .arg(getControlStateString())
                     .arg(m_altitude, 0, 'f', 0)
                     .arg(m_speed, 0, 'f', 1)
                     .arg(m_remainingOil, 0, 'f', 1)
                     .arg(m_signalStrength, 0, 'f', 1);
    
    if (!m_cameraOnline || !m_gpsOnline || !m_radarOnline) {
        tooltip += tr("\nSensors: ");
        QStringList offlineSensors;
        if (!m_cameraOnline) offlineSensors << tr("Camera");
        if (!m_gpsOnline) offlineSensors << tr("GPS");
        if (!m_radarOnline) offlineSensors << tr("Radar");
        tooltip += offlineSensors.join(", ");
    }
    
    return tooltip;
}

QPointF Airplane::projAnchor() const
{
    const QGVCameraState state = getMap()->getCamera();
    double wvalidSize = m_airIconSize.width() / state.scale();
    double hvalidSize = m_airIconSize.height() / state.scale();
    QRectF projRect(m_projPos - QPointF(wvalidSize/2, hvalidSize/2),
                    m_projPos + QPointF(wvalidSize/2, hvalidSize/2));
    return projRect.center();
}

QTransform Airplane::projTransform() const
{
    return QGV::createTransfromAzimuth(projAnchor(), m_heading);
}

void Airplane::updatePath() const
{
    m_cachedPath.clear();
    
    const QGVCameraState state = getMap()->getCamera();
    double wvalidSize = m_airIconSize.width() / state.scale();
    double hvalidSize = m_airIconSize.height() / state.scale();
    QRectF projRect(m_projPos - QPointF(wvalidSize/2, hvalidSize/2),
                    m_projPos + QPointF(wvalidSize/2, hvalidSize/2));
    
    m_cachedPath.addRect(projRect);
}

QColor Airplane::getDisplayColor() const
{
    switch (m_state) {
    case AirPlaneItem::Normal:
        return QColor(0, 255, 0);      // 绿色 - 正常
    case AirPlaneItem::Damage:
        return QColor(255, 0, 0);      // 红色 - 损坏
    case AirPlaneItem::Emergency:
        return QColor(255, 0, 255);    // 洋红色 - 紧急
    case AirPlaneItem::Maintenance:
        return QColor(255, 165, 0);    // 橙色 - 维护
    case AirPlaneItem::Offline:
        return QColor(128, 128, 128);  // 灰色 - 离线
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

QString Airplane::getStateString() const
{
    switch (m_state) {
    case AirPlaneItem::Normal: 
        return tr("Normal");
    case AirPlaneItem::Damage: 
        return tr("Damage");
    case AirPlaneItem::Emergency: 
        return tr("Emergency");
    case AirPlaneItem::Maintenance: 
        return tr("Maintenance");
    case AirPlaneItem::Offline: 
        return tr("Offline");
    default: 
        return tr("Unknown");
    }
}

QString Airplane::getControlStateString() const
{
    switch (m_controlState) {
    case AirPlaneItem::Manual:
        return tr("Manual");
    case AirPlaneItem::Auto:
        return tr("Auto");
    case AirPlaneItem::Unknown:
        return tr("Unknown");
    default:
        return tr("Unknown");
    }
}

QPen Airplane::getAirplanePen() const
{
    QColor color = getDisplayColor();
    QPen pen(color);
    const QGVCameraState state = getMap()->getCamera();
    pen.setWidthF(1.0f / state.scale());
    return pen;
}

QBrush Airplane::getAirplaneBrush() const
{
    QColor color = getDisplayColor();
    if (!m_isActive) {
        color.setAlpha(128);  // 50% opacity for inactive airplanes
    }
    return QBrush(color);
}

void Airplane::loadImages()
{
    QString normalPath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/airplane_normal.png";
    QString unknownPath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/airplane_unknown.png";
    QString damagePath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/airplane_damage.png";
    m_airNormalImage = QImage(normalPath);
    m_airUnknownImage = QImage(unknownPath);
    m_airDamageImage = QImage(damagePath);
    m_airIconSize = Settings::instance()->getValue(SettingsKey::AirplaneSize).toSizeF();
}

QColor Airplane::getTextColorForBackground(const QColor &background) const
{
    // Calculate background brightness (YIQ formula)
    double brightness = (background.red() * 299 + background.green() * 587 + background.blue() * 114) / 1000.0;
    
    // Use black text for light backgrounds, white text for dark backgrounds
    if (brightness > 128) {
        return QColor(0, 0, 0);  // Black text
    } else {
        return QColor(255, 255, 255);  // White text
    }
}

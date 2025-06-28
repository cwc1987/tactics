#include "rocket.h"
#include <QGVMap.h>
#include <QGVProjection.h>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QtMath>
#include <QDebug>
#include "settings.h"

Rocket::Rocket()
    : m_altitude(0.0)
    , m_heading(0.0)
    , m_speed(0.0)
    , m_verticalSpeed(0.0)
    , m_state(TargetItem::Normal)
    , m_threatLevel(TargetItem::Low)
    , m_confidence(0.0)
    , m_isDetected(false)
    , m_isTracked(false)
    , m_detectionRange(0.0)
    , m_weaponRange(0.0)
    , m_missileCount(0)
    , m_communicationRange(0.0)
    , m_isActive(true)
{
    loadImages();
    setFlag(QGV::ItemFlag::Transformed);
}

Rocket::Rocket(const TargetItem& targetItem)
    : m_altitude(0.0)
    , m_heading(0.0)
    , m_speed(0.0)
    , m_verticalSpeed(0.0)
    , m_state(TargetItem::Normal)
    , m_threatLevel(TargetItem::Low)
    , m_confidence(0.0)
    , m_isDetected(false)
    , m_isTracked(false)
    , m_detectionRange(0.0)
    , m_weaponRange(0.0)
    , m_missileCount(0)
    , m_communicationRange(0.0)
    , m_isActive(true)
{
    setRocketData(targetItem);
    loadImages();
    setFlag(QGV::ItemFlag::Transformed);
}

Rocket::Rocket(const QGV::GeoPos& geoPos, const double &heading, const TargetItem::TargetState &state)
    : m_geoPos(geoPos)
    , m_altitude(0.0)
    , m_heading(heading)
    , m_speed(0.0)
    , m_verticalSpeed(0.0)
    , m_state(state)
    , m_threatLevel(TargetItem::Low)
    , m_confidence(0.0)
    , m_isDetected(false)
    , m_isTracked(false)
    , m_detectionRange(0.0)
    , m_weaponRange(0.0)
    , m_missileCount(0)
    , m_communicationRange(0.0)
    , m_isActive(true)
{
    loadImages();
    setFlag(QGV::ItemFlag::Transformed);
}

Rocket::~Rocket()
{
}

void Rocket::setRocketData(const TargetItem& targetItem)
{
    m_id = targetItem.id;
    m_name = targetItem.name;
    m_model = targetItem.model;
    m_type = targetItem.type;
    m_geoPos = QGV::GeoPos(targetItem.latitude, targetItem.longitude);
    m_altitude = targetItem.altitude;
    m_heading = targetItem.heading;
    m_speed = targetItem.speed;
    m_verticalSpeed = targetItem.verticalSpeed;
    m_state = targetItem.state;
    m_threatLevel = targetItem.threatLevel;
    m_confidence = targetItem.confidence;
    m_isDetected = targetItem.isDetected;
    m_isTracked = targetItem.isTracked;
    m_detectionRange = targetItem.detectionRange;
    m_weaponRange = targetItem.weaponRange;
    m_missileCount = targetItem.missileCount;
    m_communicationType = targetItem.communicationType;
    m_communicationRange = targetItem.communicationRange;
    m_firstDetectionTime = targetItem.firstDetectionTime;
    m_lastDetectionTime = targetItem.lastDetectionTime;
    onProjection(getMap());
    resetBoundary();
    refresh();
}

void Rocket::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    m_projPos = geoMap->getProjection()->geoToProj(m_geoPos);
}

QPainterPath Rocket::projShape() const
{
    updatePath();
    return m_cachedPath;
}

void Rocket::projPaint(QPainter *painter)
{
    if (!painter) {
        return;
    }
    
    updatePath();
    
    const QGVCameraState state = getMap()->getCamera();
    double wvalidSize = m_rocketIconSize.width() / state.scale();
    double hvalidSize = m_rocketIconSize.height() / state.scale();
    QRectF projRect(m_projPos - QPointF(wvalidSize/2, hvalidSize/2),
                    m_projPos + QPointF(wvalidSize/2, hvalidSize/2));

    // Draw rocket image based on state
    if(m_state == TargetItem::Normal)
        painter->drawImage(projRect, m_rocketNormalImage);
    else if(m_state == TargetItem::Damage)
        painter->drawImage(projRect, m_rocketDamageImage);
    else if(m_state == TargetItem::Destoried)
        painter->drawImage(projRect, m_rocketDestroyedImage);
    else
        painter->drawImage(projRect, m_rocketUnknownImage);
    
    // Draw rocket label if name is not empty and active
    if (!m_name.isEmpty() && m_isActive) {
        // Set font
        QFont font = painter->font();
        font.setPointSizeF(8.0f / state.scale());
        painter->setFont(font);
        
        // Calculate text position (above the rocket)
        QPointF textPos = m_projPos + QPointF(0, -hvalidSize/2 - 5.0f / state.scale());
        
        // Calculate background color and text color
        QColor bgColor = getDisplayColor();
        bgColor.setAlpha(180); // Semi-transparent background
        
        double luminance = 0.299 * bgColor.red() + 0.587 * bgColor.green() + 0.114 * bgColor.blue();
        QColor textColor = (luminance > 140) ? Qt::black : Qt::white;
        
        // Calculate text rectangle
        QFontMetrics fm(font);
        QRect textRect = fm.boundingRect(m_name);
        textRect.moveCenter(textPos.toPoint());
        
        // Draw background rectangle
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(textRect);
        
        // Draw text
        painter->setPen(textColor);
        painter->drawText(textRect, Qt::AlignCenter, m_name);
    }
}

QString Rocket::projTooltip(const QPointF& projPos) const
{
    Q_UNUSED(projPos)
    
    QString tooltip = QString(tr("Rocket: %1\nType: %2\nModel: %3\nState: %4\nThreat Level: %5\nConfidence: %6%\nAltitude: %7 m\nSpeed: %8 m/s\nMissiles: %9"))
                     .arg(m_name.isEmpty() ? m_id : m_name)
                     .arg(getTypeString())
                     .arg(m_model.isEmpty() ? tr("Unknown") : m_model)
                     .arg(getStateString())
                     .arg(getThreatLevelString())
                     .arg(m_confidence, 0, 'f', 1)
                     .arg(m_altitude, 0, 'f', 0)
                     .arg(m_speed, 0, 'f', 1)
                     .arg(m_missileCount);
    
    if (m_isDetected) {
        tooltip += tr("\nDetection Range: %1 m").arg(m_detectionRange, 0, 'f', 0);
    }
    
    if (m_weaponRange > 0) {
        tooltip += tr("\nWeapon Range: %1 m").arg(m_weaponRange, 0, 'f', 0);
    }
    
    if (!m_communicationType.isEmpty()) {
        tooltip += tr("\nCommunication: %1 (%2 m)").arg(m_communicationType).arg(m_communicationRange, 0, 'f', 0);
    }
    
    return tooltip;
}

QPointF Rocket::projAnchor() const
{
    const QGVCameraState state = getMap()->getCamera();
    double wvalidSize = m_rocketIconSize.width() / state.scale();
    double hvalidSize = m_rocketIconSize.height() / state.scale();
    QRectF projRect(m_projPos - QPointF(wvalidSize/2, hvalidSize/2),
                    m_projPos + QPointF(wvalidSize/2, hvalidSize/2));
    return projRect.center();
}

QTransform Rocket::projTransform() const
{
    return QGV::createTransfromAzimuth(projAnchor(), m_heading);
}

void Rocket::updatePath() const
{
    m_cachedPath.clear();
    
    const QGVCameraState state = getMap()->getCamera();
    double wvalidSize = m_rocketIconSize.width() / state.scale();
    double hvalidSize = m_rocketIconSize.height() / state.scale();
    QRectF projRect(m_projPos - QPointF(wvalidSize/2, hvalidSize/2),
                    m_projPos + QPointF(wvalidSize/2, hvalidSize/2));
    
    m_cachedPath.addRect(projRect);
}

QColor Rocket::getDisplayColor() const
{
    switch (m_state) {
    case TargetItem::Normal:
        return QColor(0, 255, 0);      // 绿色 - 正常
    case TargetItem::Damage:
        return QColor(255, 0, 0);      // 红色 - 损坏
    case TargetItem::Destoried:
        return QColor(128, 128, 128);  // 灰色 - 已摧毁
    case TargetItem::UnkonwnState:
        return QColor(128, 128, 128);  // 灰色 - 未知状态
    default:
        return QColor(128, 128, 128);  // 默认灰色
    }
}

QString Rocket::getStateString() const
{
    switch (m_state) {
    case TargetItem::Normal: 
        return tr("Normal");
    case TargetItem::Damage: 
        return tr("Damage");
    case TargetItem::Destoried: 
        return tr("Destroyed");
    case TargetItem::UnkonwnState: 
        return tr("Unknown");
    default: 
        return tr("Unknown");
    }
}

QString Rocket::getThreatLevelString() const
{
    switch (m_threatLevel) {
    case TargetItem::Low:
        return tr("Low");
    case TargetItem::Medium:
        return tr("Medium");
    case TargetItem::High:
        return tr("High");
    case TargetItem::Critical:
        return tr("Critical");
    default:
        return tr("Unknown");
    }
}

QString Rocket::getTypeString() const
{
    switch (m_type) {
    case TargetItem::EnemyTank:
        return tr("Enemy Tank");
    case TargetItem::EnemyArmoredVehicle:
        return tr("Enemy Armored Vehicle");
    case TargetItem::FriendlyRocket:
        return tr("Friendly Rocket");
    case TargetItem::FriendlyGroundStation:
        return tr("Friendly Ground Station");
    case TargetItem::FriendlyProtectionVehicle:
        return tr("Friendly Protection Vehicle");
    case TargetItem::UnknownType:
        return tr("Unknown Type");
    default:
        return tr("Unknown Type");
    }
}

QPen Rocket::getRocketPen() const
{
    QColor color = getDisplayColor();
    QPen pen(color);
    const QGVCameraState state = getMap()->getCamera();
    pen.setWidthF(1.0f / state.scale());
    return pen;
}

QBrush Rocket::getRocketBrush() const
{
    QColor color = getDisplayColor();
    if (!m_isActive) {
        color.setAlpha(128);  // 50% opacity for inactive rockets
    }
    return QBrush(color);
}

void Rocket::loadImages()
{
    QString normalPath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/rocket_normal.png";
    QString damagePath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/rocket_damage.png";
    QString destroyedPath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/rocket_destroyed.png";
    QString unknownPath = Settings::instance()->getValue(SettingsKey::ItemResourcesPath).toString() + "/rocket_unknown.png";
    
    m_rocketNormalImage = QImage(normalPath);
    m_rocketDamageImage = QImage(damagePath);
    m_rocketDestroyedImage = QImage(destroyedPath);
    m_rocketUnknownImage = QImage(unknownPath);
    
    // Use rocket size setting, fallback to airplane size if not available
    QVariant rocketSize = Settings::instance()->getValue(SettingsKey::RocketSize);
    if (rocketSize.isValid() && rocketSize.canConvert<QSizeF>()) {
        m_rocketIconSize = rocketSize.toSizeF();
    } else {
        // Fallback to airplane size
        m_rocketIconSize = Settings::instance()->getValue(SettingsKey::AirplaneSize).toSizeF();
    }
}

QColor Rocket::getTextColorForBackground(const QColor &background) const
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
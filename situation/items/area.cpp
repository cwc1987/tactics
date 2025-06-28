#include "area.h"
#include "areamodel.h"
#include "settings.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontMetrics>
#include <QDebug>

Area::Area()
    : m_areaType(0)  // TaskArea
    , m_color("#FF0000")
    , m_minAltitude(0.0)
    , m_maxAltitude(10000.0)
    , m_isActive(false)
    , m_pathDirty(true)
{
    // Set default flags for area item
    //setFlag(QGV::ItemFlag::Clickable, true);
    //setFlag(QGV::ItemFlag::Highlightable, true);
}

Area::Area(const AreaItem& areaItem)
    : m_areaType(0)
    , m_color("#FF0000")
    , m_minAltitude(0.0)
    , m_maxAltitude(10000.0)
    , m_isActive(false)
    , m_pathDirty(true)
{
    setAreaData(areaItem);
    //setFlag(QGV::ItemFlag::Clickable, true);
    //setFlag(QGV::ItemFlag::Highlightable, true);
}

void Area::setAreaData(const AreaItem& areaItem)
{
    m_id = areaItem.id;
    m_name = areaItem.name;
    m_areaType = static_cast<int>(areaItem.areaType);
    m_description = areaItem.description;
    m_color = areaItem.color;
    m_minAltitude = areaItem.minAltitude;
    m_maxAltitude = areaItem.maxAltitude;
    m_isActive = areaItem.isActive;
    
    // Convert coordinates from AreaItem to QPointF
    m_coordinates.clear();
    for (const auto& coord : areaItem.coordinates) {
        m_coordinates.append(QGV::GeoPos(coord.latitude,coord.longitude));
    }
    
    m_pathDirty = true;
    update();
}

QPainterPath Area::projShape() const
{
    updatePath();
    return m_cachedPath;
}

void Area::projPaint(QPainter* painter)
{
    if (!painter || m_coordinates.size() < 3) {
        return;
    }
    
    //updatePath();
    
    // Set up painter
    painter->setRenderHint(QPainter::Antialiasing, true);
    
    // Draw area fill
    QBrush brush = getAreaBrush();
    painter->setBrush(brush);
    
    // Draw area border
    QPen pen = getAreaPen();
    painter->setPen(pen);
    
    // Draw the area path
    painter->drawPath(m_cachedPath);
    
    const QGVCameraState state = getMap()->getCamera();
    // Draw area label if name is not empty
    if (!m_name.isEmpty()) {
        // 计算几何中心
        QPointF center;
        if (m_coordinates.size() > 0) {
            double sumX = 0.0, sumY = 0.0;
            for (const auto& coord : m_coordinates) {
                QPointF proj = this->getMap()->getProjection()->geoToProj(coord);
                sumX += proj.x();
                sumY += proj.y();
            }
            center = QPointF(sumX / m_coordinates.size(), sumY / m_coordinates.size());
        } else {
            center = m_cachedPath.boundingRect().center();
        }
        
        // 设置字体
        QFont font = painter->font();
        double fontSize = Settings::instance()->getValue(SettingsKey::AreaFontSize).toDouble();
        font.setPointSizeF(fontSize / state.scale());
        painter->setFont(font);
        
        // 计算字体底色和字体色
        QColor areaColor = getDisplayColor();
        QColor bgColor = areaColor;
        bgColor.setAlpha(180);
        
        // 计算亮度
        double luminance = 0.299 * bgColor.red() + 0.587 * bgColor.green() + 0.114 * bgColor.blue();
        // 字体色：亮色背景用黑字，暗色背景用白字
        QColor textColor = (luminance > 140) ? Qt::black : Qt::white;
        
        // 计算文本矩形
        QFontMetrics fm(font);
        QRect textRect = fm.boundingRect(m_name);
        textRect.moveCenter(center.toPoint());
        
        // 绘制背景色矩形
        painter->setBrush(bgColor);
        painter->setPen(Qt::NoPen);
        painter->drawRect(textRect);
        
        // 绘制文本
        painter->setPen(textColor);
        painter->drawText(textRect, Qt::AlignCenter, m_name);
    }
}

QString Area::projTooltip(const QPointF& projPos) const
{
    Q_UNUSED(projPos)
    
    QString tooltip = QString(tr("Area: %1\nType: %2\nDescription: %3"))
                     .arg(m_name.isEmpty() ? m_id : m_name)
                     .arg(getAreaTypeString())
                     .arg(m_description.isEmpty() ? tr("No description") : m_description);
    return tooltip;
}

void Area::updatePath() const
{
    if (!m_pathDirty) {
        return;
    }
    
    m_cachedPath.clear();
    
    if (m_coordinates.size() < 3) {
        m_pathDirty = false;
        return;
    }
    
    // Create polygon path from coordinates
    QPointF projFirst = this->getMap()->getProjection()->geoToProj(m_coordinates.first());
    m_cachedPath.moveTo(projFirst);
    for (int i = 1; i < m_coordinates.size(); ++i) {
        QPointF proj = this->getMap()->getProjection()->geoToProj(m_coordinates[i]);
        m_cachedPath.lineTo(proj);
    }
    m_cachedPath.closeSubpath();
    
    m_pathDirty = false;
}

QColor Area::getDisplayColor() const
{
    QColor color(m_color);
    if (!color.isValid()) {
        color = QColor("#FF0000");
    }    
    return color;
}

QString Area::getAreaTypeString() const
{
    switch (m_areaType) {
        case 0: return tr("Task Area");
        case 1: return tr("No Fly Zone");
        case 2: return tr("Threat Zone");
        default: return tr("Unknown");
    }
}

QPen Area::getAreaPen() const
{
    QColor color = getDisplayColor();
    QPen pen(color);
    const QGVCameraState state = getMap()->getCamera();
    double penWidth = Settings::instance()->getValue(SettingsKey::AreaPenWidth).toDouble();
    pen.setWidthF(penWidth / state.scale());
    
    // Use different line styles for different area types
    switch (m_areaType) {
        case 0: // Task Area
            pen.setStyle(Qt::SolidLine);
            break;
        case 1: // No Fly Zone
            pen.setStyle(Qt::DashLine);
            break;
        case 2: // Threat Zone
            pen.setStyle(Qt::DotLine);
            break;
        default:
            pen.setStyle(Qt::SolidLine);
            break;
    }
    
    return pen;
}

QBrush Area::getAreaBrush() const
{
    QColor color = getDisplayColor();
    return QBrush(color);
}

#include "areaselectitem.h"
#include "settings.h"
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QDebug>
#include <QtMath>
#include <QKeyEvent>

AreaSelectItem::AreaSelectItem()
    : m_isSelecting(false)
    , m_isSelectionComplete(false)
    , m_selectType(0)
    , m_lineWidhtF(Settings::instance()->getValue(SettingsKey::AreaSelectLineWidth).toDouble())
    , m_pointSize(Settings::instance()->getValue(SettingsKey::AreaSelectPointSize).toDouble())
    , m_maxPoints(Settings::instance()->getValue(SettingsKey::AreaSelectMaxPoints).toInt())
{
    // Set flags for interaction
    setFlag(QGV::ItemFlag::Clickable, true);
    setFlag(QGV::ItemFlag::Trackable, true);
}

void AreaSelectItem::onProjection(QGVMap *geoMap)
{
    QGVDrawItem::onProjection(geoMap);
    if(m_isSelecting) {
        QGV::GeoRect worldGeoRect(85, -180,-85, 180);
        m_worldProjRect = geoMap->getProjection()->geoToProj(worldGeoRect);
    } else {
        QGV::GeoRect worldGeoRect(0,0,0,0);
        m_worldProjRect = geoMap->getProjection()->geoToProj(worldGeoRect);
    }
}

QPainterPath AreaSelectItem::projShape() const
{
    QPainterPath path;
    path.addRect(m_worldProjRect);
    return path;
}

void AreaSelectItem::projPaint(QPainter* painter)
{
    if (!painter) {
        return;
    }

    if (!m_isSelecting)
        return;

    // Save painter state
    painter->save();
    //Set rendering hints for better quality
    painter->setRenderHint(QPainter::Antialiasing, true);
    drawSelectionLines(painter);
    
    // // Draw preview line if selecting
    if (m_isSelecting && !m_projPoints.isEmpty()) {
        drawPreviewLine(painter);
    }
    
    // Draw selection points
    drawSelectionPoints(painter);
    
    // Restore painter state
    painter->restore();
}

void AreaSelectItem::projOnMouseClick(const QPointF& projPos, const Qt::MouseButton &button)
{
    if (!m_isSelecting) {
        return;
    }

    if(button == Qt::RightButton) {
        cancelSelection();
        return;
    } else {
        // Check if clicking near an existing point (to close the polygon)
        if (m_projPoints.size() >= 3) {
            for (int i = 0; i < m_projPoints.size(); ++i) {
                if (isPointNearExisting(projPos)) {
                    // Close the polygon by connecting to the first point
                    completeSelection();
                    return;
                }
            }
        }

        // Add new point if we haven't reached the maximum
        if (m_projPoints.size() < m_maxPoints) {
            addPoint(projPos);
        }
    }
}

void AreaSelectItem::projOnMouseDoubleClick(const QPointF& projPos)
{
    Q_UNUSED(projPos)
    if (!m_isSelecting) {
        return;
    }
    
    // Double-click to complete selection if we have enough points
    if (m_projPoints.size() >= 3) {
        completeSelection();
    }
}

void AreaSelectItem::projOnObjectTracked(const QPointF &projPos)
{
    if (!m_isSelecting) {
        return;
    }

    // Update mouse position for preview
    m_mousePos = projPos;
    repaint();
}

void AreaSelectItem::startSelection(int selectType)
{
    m_isSelecting = true;
    m_isSelectionComplete = false;
    m_selectType = selectType;
    m_projPoints.clear();
    onProjection(getMap());
    resetBoundary();
    repaint();
}

void AreaSelectItem::clearSelection()
{
    m_isSelecting = false;
    m_isSelectionComplete = false;
    m_projPoints.clear();
    onProjection(getMap());
    resetBoundary();
    repaint();
}

bool AreaSelectItem::isSelectionComplete() const
{
    return m_isSelectionComplete;
}

QVector<QPointF> AreaSelectItem::getSelectedPoints() const
{
    return m_projPoints;
}

double AreaSelectItem::getSelectionArea() const
{
    if (!m_isSelectionComplete || m_projPoints.size() < 3) {
        return 0.0;
    }
    
    // Remove the duplicate last point if it exists
    QVector<QPointF> points = m_projPoints;
    if (points.size() > 1 && points.first() == points.last()) {
        points.removeLast();
    }
    
    return calculatePolygonArea(points);
}

double AreaSelectItem::getSelectionPerimeter() const
{
    if (!m_isSelectionComplete || m_projPoints.size() < 3) {
        return 0.0;
    }
    
    // Remove the duplicate last point if it exists
    QVector<QPointF> points = m_projPoints;
    if (points.size() > 1 && points.first() == points.last()) {
        points.removeLast();
    }
    
    return calculatePolygonPerimeter(points);
}

QRectF AreaSelectItem::getSelectionBoundingBox() const
{
    if (m_projPoints.isEmpty()) {
        return QRectF();
    }
    
    // Remove the duplicate last point if it exists
    QVector<QPointF> points = m_projPoints;
    if (points.size() > 1 && points.first() == points.last()) {
        points.removeLast();
    }
    
    if (points.isEmpty()) {
        return QRectF();
    }
    
    qreal minX = points.first().x();
    qreal maxX = points.first().x();
    qreal minY = points.first().y();
    qreal maxY = points.first().y();
    
    for (const QPointF& point : points) {
        minX = qMin(minX, point.x());
        maxX = qMax(maxX, point.x());
        minY = qMin(minY, point.y());
        maxY = qMax(maxY, point.y());
    }
    
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

void AreaSelectItem::addPoint(const QPointF& point)
{
    if (m_projPoints.size() >= m_maxPoints) {
        return;
    }
    
    m_projPoints.append(point);
    repaint();
    
    qDebug() << "Added point:" << point << "Total points:" << m_projPoints.size();
}

void AreaSelectItem::removeLastPoint()
{
    if (!m_projPoints.isEmpty()) {
        m_projPoints.removeLast();
        repaint();
        
        qDebug() << "Removed last point. Total points:" << m_projPoints.size();
    }
}

void AreaSelectItem::completeSelection()
{
    if (m_projPoints.size() >= 3) {
        m_isSelectionComplete = true;
        m_isSelecting = false;
        
        // Create a closed polygon by adding the first point at the end
        if (m_projPoints.first() != m_projPoints.last()) {
            m_projPoints.append(m_projPoints.first());
        }
        
        repaint();
        
        // Emit signal with the selected points (excluding the duplicate last point)
        QVector<QPointF> points = m_projPoints;
        if (points.size() > 1 && points.first() == points.last()) {
            points.removeLast();
        }
        emit selectionCompleted(points, m_selectType);
        
        qDebug() << "Selection completed with" << points.size() << "points";
    }
}

void AreaSelectItem::cancelSelection()
{
    m_isSelecting = false;
    m_isSelectionComplete = false;
    m_projPoints.clear();
    repaint();
    emit selectionCancelled();
    
    qDebug() << "Selection cancelled";
}

void AreaSelectItem::drawSelectionLines(QPainter* painter) const
{
    if (m_projPoints.size() < 2) {
        return;
    }
    
    painter->save();
    // Set pen for selection lines
    const QGVCameraState state = getMap()->getCamera();
    QPen pen;
    QColor lineColor = Settings::instance()->getColor(SettingsKey::AreaSelectLineColor);
    pen.setColor(lineColor);
    pen.setWidthF(m_lineWidhtF / state.scale());
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    
    // Draw lines between points
    for (int i = 0; i < m_projPoints.size() - 1; ++i) {
        painter->drawLine(m_projPoints[i], m_projPoints[i + 1]);
    }
    
    // If selection is complete, draw the closing line
    if (m_isSelectionComplete && m_projPoints.size() >= 3) {
        painter->drawLine(m_projPoints.last(), m_projPoints.first());
    }
    painter->restore();
}

void AreaSelectItem::drawPreviewLine(QPainter* painter) const
{
    if (m_projPoints.isEmpty()) {
        return;
    }
    
    // Set pen for preview line (dashed, lighter color
    const QGVCameraState state = getMap()->getCamera();
    QPen pen;
    QColor penColor = Settings::instance()->getColor(SettingsKey::AreaSelectPreviewLineColor);
    pen.setColor(penColor);
    double penWidth = Settings::instance()->getValue(SettingsKey::AreaSelectPreviewLineWidth).toDouble();
    pen.setWidth(penWidth / state.scale());
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    
    // Draw preview line from last point to current mouse position
    painter->drawLine(m_projPoints.last(), m_mousePos);
}

void AreaSelectItem::drawSelectionPoints(QPainter* painter) const
{
    if (m_projPoints.isEmpty()) {
        return;
    }
    
    painter->save();
    QBrush brush(Settings::instance()->getColor(SettingsKey::AreaSelectPointColor));
    painter->setBrush(brush);
    for(int i = 0, iend = m_projPoints.size(); i < iend; i++) {
        const QGVCameraState state = getMap()->getCamera();
        double wvalidSize = m_pointSize / state.scale();
        double hvalidSize = m_pointSize/ state.scale();
        QRectF projRect(m_projPoints[i] - QPointF(wvalidSize/2, hvalidSize/2),
                        m_projPoints[i] + QPointF(wvalidSize/2, hvalidSize/2));
        painter->drawEllipse(projRect);
    }
    painter->restore();
}

bool AreaSelectItem::isPointNearExisting(const QPointF& point) const
{
    const QGVCameraState state = getMap()->getCamera();
    const qreal threshold = 10.0 / state.scale(); // Distance threshold in pixels
    
    for (const QPointF& existingPoint : m_projPoints) {
        qreal distance = QLineF(point, existingPoint).length();
        if (distance <= threshold) {
            return true;
        }
    }
    
    return false;
}

double AreaSelectItem::calculatePolygonArea(const QVector<QPointF>& points) const
{
    if (points.size() < 3) {
        return 0.0;
    }
    
    // Use the shoelace formula (surveyor's formula) to calculate polygon area
    double area = 0.0;
    int n = points.size();
    
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        area += points[i].x() * points[j].y();
        area -= points[j].x() * points[i].y();
    }
    
    return qAbs(area) / 2.0;
}

double AreaSelectItem::calculatePolygonPerimeter(const QVector<QPointF>& points) const
{
    if (points.size() < 2) {
        return 0.0;
    }
    
    double perimeter = 0.0;
    int n = points.size();
    
    for (int i = 0; i < n; ++i) {
        int j = (i + 1) % n;
        perimeter += QLineF(points[i], points[j]).length();
    }
    
    return perimeter;
}

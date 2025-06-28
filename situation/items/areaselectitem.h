#ifndef AREASELECTITEM_H
#define AREASELECTITEM_H

#include <QGVDrawItem.h>
#include <QPainterPath>
#include <QVector>
#include <QPointF>

class AreaSelectItem : public QGVDrawItem
{
    Q_OBJECT
public:
    explicit AreaSelectItem();

    // Override virtual methods from QGVDrawItem
    void onProjection(QGVMap* geoMap) override;
    QPainterPath projShape() const override;
    void projPaint(QPainter* painter) override;
    void projOnMouseClick(const QPointF& projPos, const Qt::MouseButton &button) override;
    void projOnMouseDoubleClick(const QPointF& projPos) override;
    void projOnObjectTracked(const QPointF& projPos) override;

    // Area selection control methods
    void startSelection(int selectType);
    void clearSelection();
    bool isSelectionComplete() const;
    QVector<QPointF> getSelectedPoints() const;
    
    // Get selection area information
    double getSelectionArea() const;
    double getSelectionPerimeter() const;
    QRectF getSelectionBoundingBox() const;

signals:
    void selectionCompleted(const QVector<QPointF>& points, int type);
    void selectionCancelled();

private:    
    QVector<QPointF> m_projPoints;    // Projected points for the selection area
    QPointF m_mousePos;               // Current mouse position for preview
    bool m_isSelecting;               // Whether currently in selection mode
    bool m_isSelectionComplete;       // Whether selection is complete (closed polygon)
    int  m_selectType;
    QRectF m_worldProjRect;
    double m_lineWidhtF;
    double m_pointSize;
    int  m_maxPoints;
    
    // Helper methods
    void addPoint(const QPointF& point);
    void removeLastPoint();
    void completeSelection();
    void cancelSelection();
    void drawSelectionLines(QPainter* painter) const;
    void drawSelectionPoints(QPainter* painter) const;
    void drawPreviewLine(QPainter* painter) const;
    bool isPointNearExisting(const QPointF& point) const;
    double calculatePolygonArea(const QVector<QPointF>& points) const;
    double calculatePolygonPerimeter(const QVector<QPointF>& points) const;
};

#endif // AREASELECTITEM_H
